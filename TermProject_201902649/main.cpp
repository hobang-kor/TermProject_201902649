#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

class Singleton {
private:
	vector<string> file;	//���κ� ���ڿ��� �����ϴ� vector
	string msg = "";	//�ָܼ޼��� ���
	int page = 0;	//���� ������
	static Singleton* s_instance;	//SingletonŬ������ �ν��Ͻ� ����
	Singleton() {}	//private�� �����ڸ� ����

public:
	static Singleton* instance() {	//SingletonŬ������ �����ϱ����� �Լ�
		if (!s_instance) {	//SingletonŬ������ �ν��Ͻ��� ���ٸ�
			s_instance = new Singleton();	//SingletonŬ���� �ν��Ͻ��� �����
		}
		return s_instance;	//SingletonŬ�����ν��Ͻ��� ��ȯ�Ѵ�.
	}
	vector<string> getFile() {
		return file;	//���κ� ���ڿ��� �����ϴ� vector�� ��ȯ
	}
	int getPage() {
		return page;	//���� �������� ��ȯ
	}
	string getMsg() {
		return msg;		//���� �ָܼ޼����� ��ȯ
	}
	void setFile(vector<string> file) {
		this->file = file;	//���κ� ���ڿ��� �����ϴ� vector�� �ʱ�ȭ
	}
	void setPage(int page) {
		this->page = page;	//�������� �ʱ�ȭ
	}
	void setMsg(string msg) {
		this->msg = msg;	//�ָܼ޼����� �ʱ�ȭ
	}
};
Singleton* Singleton::s_instance = 0;	//ó�� ������ �� SingletonŬ������ �ν��Ͻ��� 0

class MyText {
private:
	vector<string> orderVector;		//������� ����� vector�� ����

	vector<string> stringToOrder(string order) {	//��ɹ��� �Է��� ���ڿ��� ��ɰ� ���ڵ��� �̾Ƽ� vector�� �����Ѵ�.
		string tmp;
		vector<string> v_order;	//�Ͻ����� ������� ��� ����
		for (int i = 0; i < order.length(); i++) {	//���ڿ��� ���̸�ŭ
			if (order[i] == '(') {		// "("�̶�� tmp���� ����� ����Ǿ� �ִ�.
				v_order.push_back(tmp);
				tmp = "";
			}
			else if (order[i] == ',') {	// ","�̶�� tmp���� ���ڰ� ����Ǿ� �ִ�.
				v_order.push_back(tmp);
				tmp = "";
			}
			else if (order[i] == ')') {	// ")"�̶�� tmp���� ������ ���ڰ� ����Ǿ� �ִ�.
				v_order.push_back(tmp);
				tmp = "";
			}
			else {
				tmp += order[i];	//����̳� ���ڸ� tmp�� ��´�
			}
		}
		if (tmp.length() > 0) {
			v_order.push_back(tmp);	//t, n, p����� ��ȣ, ��ǥ�� ������� �����Ƿ� ���� ó��
		}
		return v_order;
	}

	void page_print() {	//���� �������� �ش��ϴ� 20���� ������ ������ִ� �Լ�
		int page = Singleton::instance()->getPage();	 //Singleton�ν��Ͻ��� �������� �ҷ��´�
		vector<string> file = Singleton::instance()->getFile(); //Singleton�ν��Ͻ��� ������ �ҷ��´�
		if (page + 20 >= file.size()) {
			page = file.size() - 20;	// 20������ �ִ� ������ �̵�, ��, ���� ��� ���뿡�� ���Ӱ�
			//����� ���� ���� 4�� ���� ������ ���, ���� ȭ���� ������ 16�� ���� + ���ο� 4�� ������ �߰� ���
		}
		else if (page < 0) {
			page = 0;	// �������� 0���� �۾����� 0����
		}
		for (int i = 0; i < 20; i++) {
			if (i < 9) {
				cout << " " << i + 1 << "| ";
			}
			else {
				cout << i + 1 << "| ";
			}
			cout << file[page + i] << endl;
		}
		Singleton::instance()->setPage(page);	 //Singleton�ν��Ͻ��� �������� ������Ʈ
	}

	void order_i(int a, int b, string c) {	//���Ը�ɿ� ���� �Լ�
		vector<string> tmp_vector;
		string tmp_string;
		vector<string> file = Singleton::instance()->getFile();	//Singleton�ν��Ͻ��� ������ �ҷ��´�

		for (int i = 0; i <= a - 2; i++) {	//������ ���� �������� ������ �״��
			tmp_vector.push_back(file[i]);
		}
		tmp_string = file[a - 1];	//������ ���ΰ� �� ���� ���� ���ؼ��� ������ �ִ� ����Ʈ�� �Ѵ��� Ȯ���ؾ� �Ѵ�.
		tmp_string.insert(b, c);	//�ش� ���� ���ڿ��� ����ִ� tmp_string�� insert�� ���ڿ� ����
		for (int j = a; j < file.size(); j++) {	//�� ������ ���δ���
			tmp_string += " " + file[j];	//������ tmp_string�� �߰��ϰ�
			if (tmp_string.length() > 75) {	//�ִ� ����Ʈ�� 75�� �Ѵ´ٸ�
				int id = 75;
				while (id > 0) {	//�ڿ������� ó�� ������ ���� ������ ����
					if (tmp_string[id] == ' ') {
						if (tmp_string[0] == ' ') {	//�������� �����Ͽ��ٸ� �� ���� ������ ���ش�
							tmp_vector.push_back(tmp_string.substr(1, id));	//���� �������� ���ο� ��������
						}
						else {
							tmp_vector.push_back(tmp_string.substr(0, id));	//�������� �������� �ʾ��� ��
						}

						tmp_string = tmp_string.substr(id + 1, tmp_string.length() - 1);	//���� tmp_string
						break;
					}
					id--;
				}
			}
		}
		if (tmp_string.length() > 0) { tmp_vector.push_back(tmp_string); }	//tmp_string�� ���̰� 0���� ũ�ٸ� �����ִ� ���ڿ��� vector�� ����
		Singleton::instance()->setFile(tmp_vector);	//Singleton�ν��Ͻ��� �ش� ������ ������Ʈ
	}

	void order_d(int a, int b, int c) {	//���� ��ɿ� ���� �Լ�
		vector<string> file = Singleton::instance()->getFile();	//Singleton�ν��Ͻ��� ������ �ҷ��´�
		int page = Singleton::instance()->getPage();	//Singleton�ν��Ͻ��� �������� �ҷ��´�
		int d_line = page + a - 1;
		int index = b;
		file[d_line].erase(index, index + c);	//�ش� ���ο� �ε������� �����Ͽ�
		Singleton::instance()->setFile(file);	//Singleton�ν��Ͻ��� ������ ������Ʈ
	}

	bool order_s(string a) {	//ã�� ��ɿ� ���� �Լ�
		vector<string> tmp_vector;
		string tmp_string;
		vector<string> file = Singleton::instance()->getFile();	//Singleton�ν��Ͻ��� ������ �ҷ��´�
		int page = Singleton::instance()->getPage();	//Singleton�ν��Ͻ��� �������� �ҷ��´�
		bool wordFind = false;	//�ش� ���Ͽ� ���ڰ� ����ִ����� ���� ����
		for (int i = 0; i < file.size(); i++) {	//���� �����ŭ �ݺ�
			if (file[i].find(a) != string::npos) {	//�ش� ���ο��� ���ڸ� ã�Ҵٸ�
				wordFind = true;	//���Ͽ� ���ڰ� ��������Ƿ� true
				page = i + 1;		//ù��° ���� index=0�� ��ġ�ϵ��� ȭ���� �ٽ� ����ؾ� �ϹǷ� page�� ����
				int index = file[i].find(a);	//�� ������ �ε���
				if (index == 0) {	//�ε����� 0�̶�� �� ó���� �ִٴ� ���̹Ƿ�
					page = i;	//page�� �״�� i 
				}
				else {
					tmp_vector.push_back(file[i].substr(0, index));	//�� ���ڿ� �ձ��� �������� ����
				}
				tmp_string = file[i].substr(index, file[i].length());	//�ش� ���ڿ����� ���ο� ��������(page = i + 1�� ���� ����)
				for (int j = i + 1; j < file.size(); j++) {	//���� ���κ��� ������
					tmp_string += " " + file[j];	//���⼭���ʹ� ���� ��ɰ� �Ȱ���
					if (tmp_string.length() > 75) {
						int id = 75;
						while (id > 0) {
							if (tmp_string[id] == ' ') {
								if (tmp_string[0] == ' ') {
									tmp_vector.push_back(tmp_string.substr(1, id));
								}
								else {
									tmp_vector.push_back(tmp_string.substr(0, id));
								}

								tmp_string = tmp_string.substr(id + 1, tmp_string.length() - 1);
								break;
							}
							id--;
						}
					}
				}
				tmp_vector.push_back(tmp_string);
				break;
			}
			tmp_vector.push_back(file[i]);
		}
		Singleton::instance()->setFile(tmp_vector);	//Singleton�ν��Ͻ��� ������ ������Ʈ
		Singleton::instance()->setPage(page);	//Singleton�ν��Ͻ��� �������� ������Ʈ
		return wordFind;	//�ش� ���Ͽ� ���ڰ� ����ִ����� ���� ������ ���� ��ȯ
	}

	int order_c(string a, string b) {	//���� ��ɿ� ���� �Լ�
		int stateOfC = 0;	//�����ɿ��� 3���� ������ ����
		//0: �ش� ���ڿ��� �������� �ʴ´�
		//1: �ش� ���ڿ��� �����Ͽ��� �� ������ 75����Ʈ�� �Ѵ´�
		//2: ���������� ����
		vector<string> file = Singleton::instance()->getFile();	//Singleton�ν��Ͻ��� ������ �ҷ��´�
		string tmp_string;
		for (int i = 0; i < file.size(); i++) {
			if (file[i].find(a) != string::npos) {	//�ش� ���ڿ��� ã�Ҵٸ� 
				stateOfC = 2;
				int index = file[i].find(a);
				file[i].replace(index, a.length(), b);	//�ش� ���ڸ� �����ϰ� ���� ���ڿ��� ��ü
				if (file[i].length() > 75) {
					stateOfC = 1;
					return stateOfC;
				}
			}
		}
		Singleton::instance()->setFile(file);	//Singleton�ν��Ͻ��� ������ ������Ʈ
		return stateOfC;
	}

	void order_t() {	//���� �� ���� ��ɿ� ���� �Լ�
		vector<string> file = Singleton::instance()->getFile();	//Singleton�ν��Ͻ��� ������ �ҷ��´�
		ofstream newfile;
		newfile.open("test.txt");
		for (int i = 0; i < file.size(); i++) {
			string s = file[i];
			s += '\n';
			newfile.write(s.c_str(), s.size());		//����� ������ ���Ͽ� ����
		}
		newfile.close();
	}

	void order_n() {	//���� ������ ��ɿ� ���� �Լ�
		Singleton::instance()->setPage(Singleton::instance()->getPage() + 20);	//Singleton�ν��Ͻ��� �������� ������Ʈ
	}

	void order_p() {	//���� ������ ��ɿ� ���� �Լ�
		Singleton::instance()->setPage(Singleton::instance()->getPage() - 20);	//Singleton�ν��Ͻ��� �������� ������Ʈ
	}
public:
	void setSingleton() {	//Singleton��ü�� ����
		int page = 0;	//ó�� �������� 0
		string msg = "";	//ó�� �ܼ� �޼����� ""
		ifstream readfile;	//���� �б�
		vector<string> file;
		readfile.open("test.txt");	//������ �о�´�
		if (readfile.is_open()) {
			string line = "";	//���Ϳ� �����ϱ� ����
			while (!readfile.eof()) {
				string tmp;
				readfile >> tmp;
				if (line.length() + tmp.length() >= 75) {	//75����Ʈ�� �Ѿ��
					file.push_back(line.substr(0, line.length()));	//�������� ����ִ� ������ ����
					line = "";
				}
				line += tmp + " ";
			}
			file.push_back(line);	//vector�� ������ ����
			readfile.close();
		}
		else {
			readfile.close();
		}
		Singleton::instance();
		Singleton::instance()->setFile(file);
		Singleton::instance()->setMsg(msg);
		Singleton::instance()->setPage(page);	////Singleton�ν��Ͻ� ������Ʈ;
	}

	void inputOrder() {	//����ڰ� ���ԵǴ� �ܼ� â
		while (true) {
			string order;	//����� �Է�
			page_print();	//�������� ���
			cout << "--------------------------------------------------------------------------" << endl;
			cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
			cout << "--------------------------------------------------------------------------" << endl;
			cout << Singleton::instance()->getMsg() << endl;	////Singleton�ν��Ͻ��� �ܼ� �޼����� ���
			cout << "--------------------------------------------------------------------------" << endl;
			cout << "�Է�:";
			cin >> order;	//��ɿ� ���� ���ڿ��� ������
			orderVector = stringToOrder(order);	//���ڿ��� vector���·� ��ȯ
			cout << "--------------------------------------------------------------------------" << endl;
			if (orderVector[0] == "i") {	//���� ����� ��
				if (orderVector.size() != 4) {	//���ڰ� 3������ �Ѵ� (����, �ε���, ������ ���ڿ�)
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					try {	//std::stoi�� �������·� �ٲ� �� �ش� ���ڿ��� ������ �ƴ϶�� catch������
						int line = std::stoi(orderVector[1]);	//std::stoi�� ����� ���ڿ��� ����������
						int idx = std::stoi(orderVector[2]);	//std::stoi�� ����� ���ڿ��� ����������
						int lineLength = Singleton::instance()->getFile()[Singleton::instance()->getPage() + line].length();	//�ش��ϴ� ������ ����
						if (line < 1 || line > 20) {
							Singleton::instance()->setMsg("Line�� ������ 1���� 20���� �Դϴ�.");
						}
						else if (idx > lineLength || idx < 0) {
							Singleton::instance()->setMsg("index�� ������ 0���� �ش� ���� �������� �Դϴ�.");
						}
						else if (orderVector[3].length() > 75) {
							Singleton::instance()->setMsg("���԰����� ���ڿ� �ִ� ������� 75�Դϴ�.");
						}
						else {
							order_i(line, idx, orderVector[3]);
							Singleton::instance()->setMsg("");
						}
					}
					catch (std::invalid_argument const& e) {	//1, 2��° ���ڰ� �������� �ƴ϶��
						Singleton::instance()->setMsg("�߸��� �Է��Դϴ�. ù��° ���ڿ� �ι�° ���ڷ� �������� �Է����ּ���");
					}
				}
			}
			else if (orderVector[0] == "d") {	//���� ����� ��
				if (orderVector.size() != 4) {	//���ڰ� 3������ �Ѵ�(����, �ε���, ������ ����Ʈ)
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					try {
						int line = std::stoi(orderVector[1]);	//std::stoi�� ����� ���ڿ��� ����������
						int idx = std::stoi(orderVector[2]);	//std::stoi�� ����� ���ڿ��� ����������
						int lineLength = Singleton::instance()->getFile()[Singleton::instance()->getPage() + line].length();	//�ش��ϴ� ������ ����
						int deByte = std::stoi(orderVector[3]);	//std::stoi�� ����� ���ڿ��� ����������

						if (line < 1 || line > 20) {
							Singleton::instance()->setMsg("Line�� ������ 1���� 20���� �Դϴ�.");
						}
						else if (idx > lineLength || idx < 0) {
							Singleton::instance()->setMsg("index�� ������ 0���� �ش� ���� �������� �Դϴ�.");
						}
						else if (idx + deByte > lineLength) {
							Singleton::instance()->setMsg("������ ����Ʈ�� �����ϴ�.");
						}
						else {
							order_d(line, idx, deByte);
							Singleton::instance()->setMsg("");
						}
					}
					catch (std::invalid_argument const& e) {
						Singleton::instance()->setMsg("�߸��� �Է��Դϴ�. ù��° ���ڿ� �ι�° ���ڿ� ����° ���ڷ� �������� �Է����ּ���");
					}
				}
			}
			else if (orderVector[0] == "s") {	//ã�� ����� ��
				if (orderVector.size() != 2) {	//���ڰ� 1������ �Ѵ�(ã�� ���ڿ�)
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					if (!order_s(orderVector[1])) {
						Singleton::instance()->setMsg("���Ͽ� �������� �ʴ� �ܾ��Դϴ�.");
					}
					else {
						Singleton::instance()->setMsg("");
					}
				}
			}
			else if (orderVector[0] == "c") {	//���� ����� ��
				if (orderVector.size() != 3) {	//���ڰ� 2������ �Ѵ�(�ش� ���ڿ�, �ٲ� ���ڿ�)
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					int oc = order_c(orderVector[1], orderVector[2]);
					if (oc == 0) {
						Singleton::instance()->setMsg("�ٲ� �ܾ ���Ͽ� �������� �ʽ��ϴ�.");
					}
					else if (oc == 1) {
						Singleton::instance()->setMsg("������ �ִ� ����Ʈ�� �Ѿ�ϴ�.");
					}
					else {
						Singleton::instance()->setMsg("");
					}
				}
			}
			else if (orderVector[0] == "t") {	//���� �� ���� ����� ��
				if (orderVector.size() != 1) {	//���ڰ� ������ �ȵȴ�.
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					order_t();
					break;
				}
			}
			else if (orderVector[0] == "n") {	//���� ������ ����� ��
				if (orderVector.size() != 1) {	//���ڰ� ������ �ȵȴ�
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					if (Singleton::instance()->getPage() == Singleton::instance()->getFile().size() - 20) {
						Singleton::instance()->setMsg("�̹� ������ ������ �Դϴ�.");	//������ ���������� n�� �Է��ϸ� �ָܼ޼��� ������Ʈ
					}
					else {
						Singleton::instance()->setMsg("");
						order_n();
					}
				}
			}
			else if (orderVector[0] == "p") {	//���� ������ ����� ��
				if (orderVector.size() != 1) {	//���ڰ� ������ �ȵȴ�
					Singleton::instance()->setMsg("������ ������ ���� �ʽ��ϴ�.");
				}
				else {
					if (Singleton::instance()->getPage() == 0) {
						Singleton::instance()->setMsg("�̹� ù��° ������ �Դϴ�.");	//ó�� ���������� p�� �Է��ϸ� �ָܼ޼��� ������Ʈ
					}
					else {
						Singleton::instance()->setMsg("");
						order_p();
					}
				}
			}
			else {	//����� i, d, s, c, t, n, p�� �ش����� �ʴ´ٸ�
				Singleton::instance()->setMsg("�߸��� ��ɾ� �Դϴ�.");	//�ָܼ޼��� ������Ʈ
			}
		}
	}
};

int main() {
	MyText mt;
	mt.setSingleton();
	mt.inputOrder();
	return 0;
}