#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

class Singleton {
private:
	vector<string> file;	//라인별 문자열을 저장하는 vector
	string msg = "";	//콘솔메세지 출력
	int page = 0;	//현재 페이지
	static Singleton* s_instance;	//Singleton클래스의 인스턴스 유무
	Singleton() {}	//private로 생성자를 만듦

public:
	static Singleton* instance() {	//Singleton클래스를 생성하기위한 함수
		if (!s_instance) {	//Singleton클래스의 인스턴스가 없다면
			s_instance = new Singleton();	//Singleton클래스 인스턴스를 만든다
		}
		return s_instance;	//Singleton클래스인스턴스를 반환한다.
	}
	vector<string> getFile() {
		return file;	//라인별 문자열을 저장하는 vector를 반환
	}
	int getPage() {
		return page;	//현재 페이지를 반환
	}
	string getMsg() {
		return msg;		//현재 콘솔메세지를 반환
	}
	void setFile(vector<string> file) {
		this->file = file;	//라인별 문자열을 저장하는 vector를 초기화
	}
	void setPage(int page) {
		this->page = page;	//페이지를 초기화
	}
	void setMsg(string msg) {
		this->msg = msg;	//콘솔메세지를 초기화
	}
};
Singleton* Singleton::s_instance = 0;	//처음 시작할 때 Singleton클래스의 인스턴스는 0

class MyText {
private:
	vector<string> orderVector;		//사용자의 명령을 vector로 저장

	vector<string> stringToOrder(string order) {	//명령문에 입력한 문자열을 명령과 인자들을 뽑아서 vector에 저장한다.
		string tmp;
		vector<string> v_order;	//일시적인 사용자의 명령 벡터
		for (int i = 0; i < order.length(); i++) {	//문자열의 길이만큼
			if (order[i] == '(') {		// "("이라면 tmp에는 명령이 저장되어 있다.
				v_order.push_back(tmp);
				tmp = "";
			}
			else if (order[i] == ',') {	// ","이라면 tmp에는 인자가 저장되어 있다.
				v_order.push_back(tmp);
				tmp = "";
			}
			else if (order[i] == ')') {	// ")"이라면 tmp에는 마지막 인자가 저장되어 있다.
				v_order.push_back(tmp);
				tmp = "";
			}
			else {
				tmp += order[i];	//명령이나 인자를 tmp에 담는다
			}
		}
		if (tmp.length() > 0) {
			v_order.push_back(tmp);	//t, n, p명령은 괄호, 쉼표가 들어있지 않으므로 따로 처리
		}
		return v_order;
	}

	void page_print() {	//현재 페이지에 해당하는 20개의 라인을 출력해주는 함수
		int page = Singleton::instance()->getPage();	 //Singleton인스턴스의 페이지를 불러온다
		vector<string> file = Singleton::instance()->getFile(); //Singleton인스턴스의 파일을 불러온다
		if (page + 20 >= file.size()) {
			page = file.size() - 20;	// 20라인을 최대 단위로 이동, 즉, 현재 출력 내용에서 새롭게
			//출력할 라인 수가 4개 라인 남았을 경우, 현재 화면의 마지막 16개 라인 + 새로운 4개 라인을 추가 출력
		}
		else if (page < 0) {
			page = 0;	// 페이지가 0보다 작아지면 0으로
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
		Singleton::instance()->setPage(page);	 //Singleton인스턴스의 페이지를 업데이트
	}

	void order_i(int a, int b, string c) {	//삽입명령에 대한 함수
		vector<string> tmp_vector;
		string tmp_string;
		vector<string> file = Singleton::instance()->getFile();	//Singleton인스턴스의 파일을 불러온다

		for (int i = 0; i <= a - 2; i++) {	//삽입할 라인 전까지는 내용이 그대로
			tmp_vector.push_back(file[i]);
		}
		tmp_string = file[a - 1];	//삽입할 라인과 그 이후 라인 대해서는 라인의 최대 바이트를 넘는지 확인해야 한다.
		tmp_string.insert(b, c);	//해당 라인 문자열이 들어있는 tmp_string에 insert로 문자열 삽입
		for (int j = a; j < file.size(); j++) {	//그 이후의 라인대해
			tmp_string += " " + file[j];	//라인을 tmp_string에 추가하고
			if (tmp_string.length() > 75) {	//최대 바이트인 75를 넘는다면
				int id = 75;
				while (id > 0) {	//뒤에서부터 처음 공백이 나올 때까지 조사
					if (tmp_string[id] == ' ') {
						if (tmp_string[0] == ' ') {	//공백으로 시작하였다면 맨 앞의 공백을 빼준다
							tmp_vector.push_back(tmp_string.substr(1, id));	//공백 전까지만 새로운 라인으로
						}
						else {
							tmp_vector.push_back(tmp_string.substr(0, id));	//공백으로 시작하지 않았을 때
						}

						tmp_string = tmp_string.substr(id + 1, tmp_string.length() - 1);	//남은 tmp_string
						break;
					}
					id--;
				}
			}
		}
		if (tmp_string.length() > 0) { tmp_vector.push_back(tmp_string); }	//tmp_string의 길이가 0보다 크다면 남아있는 문자열도 vector에 삽입
		Singleton::instance()->setFile(tmp_vector);	//Singleton인스턴스의 해당 파일을 업데이트
	}

	void order_d(int a, int b, int c) {	//삭제 명령에 대한 함수
		vector<string> file = Singleton::instance()->getFile();	//Singleton인스턴스의 파일을 불러온다
		int page = Singleton::instance()->getPage();	//Singleton인스턴스의 페이지를 불러온다
		int d_line = page + a - 1;
		int index = b;
		file[d_line].erase(index, index + c);	//해당 라인에 인덱스부터 삭제하여
		Singleton::instance()->setFile(file);	//Singleton인스턴스의 파일을 업데이트
	}

	bool order_s(string a) {	//찾기 명령에 대한 함수
		vector<string> tmp_vector;
		string tmp_string;
		vector<string> file = Singleton::instance()->getFile();	//Singleton인스턴스의 파일을 불러온다
		int page = Singleton::instance()->getPage();	//Singleton인스턴스의 페이지를 불러온다
		bool wordFind = false;	//해당 파일에 문자가 들어있는지에 대한 정보
		for (int i = 0; i < file.size(); i++) {	//파일 사이즈만큼 반복
			if (file[i].find(a) != string::npos) {	//해당 라인에서 문자를 찾았다면
				wordFind = true;	//파일에 문자가 들어있으므로 true
				page = i + 1;		//첫번째 라인 index=0에 위치하도록 화면을 다시 출력해야 하므로 page값 변경
				int index = file[i].find(a);	//그 문자의 인덱스
				if (index == 0) {	//인덱스가 0이라면 맨 처음에 있다는 뜻이므로
					page = i;	//page는 그대로 i 
				}
				else {
					tmp_vector.push_back(file[i].substr(0, index));	//그 문자열 앞까지 라인으로 저장
				}
				tmp_string = file[i].substr(index, file[i].length());	//해당 문자열부터 새로운 라인으로(page = i + 1로 해준 이유)
				for (int j = i + 1; j < file.size(); j++) {	//다음 라인부터 끝까지
					tmp_string += " " + file[j];	//여기서부터는 삽입 명령과 똑같다
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
		Singleton::instance()->setFile(tmp_vector);	//Singleton인스턴스의 파일을 업데이트
		Singleton::instance()->setPage(page);	//Singleton인스턴스의 페이지를 업데이트
		return wordFind;	//해당 파일에 문자가 들어있는지에 대한 정보에 대해 반환
	}

	int order_c(string a, string b) {	//변경 명령에 대한 함수
		int stateOfC = 0;	//변경명령에는 3가지 종류가 존재
		//0: 해당 문자열이 존재하지 않는다
		//1: 해당 문자열을 변경하였을 때 라인이 75바이트를 넘는다
		//2: 성공적으로 변경
		vector<string> file = Singleton::instance()->getFile();	//Singleton인스턴스의 파일을 불러온다
		string tmp_string;
		for (int i = 0; i < file.size(); i++) {
			if (file[i].find(a) != string::npos) {	//해당 문자열을 찾았다면 
				stateOfC = 2;
				int index = file[i].find(a);
				file[i].replace(index, a.length(), b);	//해당 문자를 변경하고 싶은 문자열로 대체
				if (file[i].length() > 75) {
					stateOfC = 1;
					return stateOfC;
				}
			}
		}
		Singleton::instance()->setFile(file);	//Singleton인스턴스의 파일을 업데이트
		return stateOfC;
	}

	void order_t() {	//저장 후 종료 명령에 대한 함수
		vector<string> file = Singleton::instance()->getFile();	//Singleton인스턴스의 파일을 불러온다
		ofstream newfile;
		newfile.open("test.txt");
		for (int i = 0; i < file.size(); i++) {
			string s = file[i];
			s += '\n';
			newfile.write(s.c_str(), s.size());		//변경된 내용을 파일에 저장
		}
		newfile.close();
	}

	void order_n() {	//다음 페이지 명령에 대한 함수
		Singleton::instance()->setPage(Singleton::instance()->getPage() + 20);	//Singleton인스턴스의 페이지를 업데이트
	}

	void order_p() {	//이전 페이지 명령에 대한 함수
		Singleton::instance()->setPage(Singleton::instance()->getPage() - 20);	//Singleton인스턴스의 페이지를 업데이트
	}
public:
	void setSingleton() {	//Singleton객체를 생성
		int page = 0;	//처음 페이지는 0
		string msg = "";	//처음 콘솔 메세지는 ""
		ifstream readfile;	//파일 읽기
		vector<string> file;
		readfile.open("test.txt");	//파일을 읽어온다
		if (readfile.is_open()) {
			string line = "";	//벡터에 저장하기 위함
			while (!readfile.eof()) {
				string tmp;
				readfile >> tmp;
				if (line.length() + tmp.length() >= 75) {	//75바이트를 넘어서면
					file.push_back(line.substr(0, line.length()));	//마지막에 들어있는 공백을 제거
					line = "";
				}
				line += tmp + " ";
			}
			file.push_back(line);	//vector에 라인을 저장
			readfile.close();
		}
		else {
			readfile.close();
		}
		Singleton::instance();
		Singleton::instance()->setFile(file);
		Singleton::instance()->setMsg(msg);
		Singleton::instance()->setPage(page);	////Singleton인스턴스 업데이트;
	}

	void inputOrder() {	//사용자가 보게되는 콘솔 창
		while (true) {
			string order;	//사용자 입력
			page_print();	//페이지를 출력
			cout << "--------------------------------------------------------------------------" << endl;
			cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
			cout << "--------------------------------------------------------------------------" << endl;
			cout << Singleton::instance()->getMsg() << endl;	////Singleton인스턴스의 콘솔 메세지를 출력
			cout << "--------------------------------------------------------------------------" << endl;
			cout << "입력:";
			cin >> order;	//명령에 대한 문자열이 들어오면
			orderVector = stringToOrder(order);	//문자열을 vector형태로 변환
			cout << "--------------------------------------------------------------------------" << endl;
			if (orderVector[0] == "i") {	//삽입 명령일 때
				if (orderVector.size() != 4) {	//인자가 3개여야 한다 (라인, 인덱스, 삽입할 문자열)
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					try {	//std::stoi로 정수형태로 바꿀 때 해당 문자열이 정수가 아니라면 catch문으로
						int line = std::stoi(orderVector[1]);	//std::stoi를 사용해 문자열을 정수형으로
						int idx = std::stoi(orderVector[2]);	//std::stoi를 사용해 문자열을 정수형으로
						int lineLength = Singleton::instance()->getFile()[Singleton::instance()->getPage() + line].length();	//해당하는 라인의 범위
						if (line < 1 || line > 20) {
							Singleton::instance()->setMsg("Line의 범위는 1부터 20까지 입니다.");
						}
						else if (idx > lineLength || idx < 0) {
							Singleton::instance()->setMsg("index의 범위는 0부터 해당 라인 범위까지 입니다.");
						}
						else if (orderVector[3].length() > 75) {
							Singleton::instance()->setMsg("삽입가능한 문자열 최대 사이즈는 75입니다.");
						}
						else {
							order_i(line, idx, orderVector[3]);
							Singleton::instance()->setMsg("");
						}
					}
					catch (std::invalid_argument const& e) {	//1, 2번째 인자가 정수형이 아니라면
						Singleton::instance()->setMsg("잘못된 입력입니다. 첫번째 인자와 두번째 인자로 정수형을 입력해주세요");
					}
				}
			}
			else if (orderVector[0] == "d") {	//삭제 명령일 때
				if (orderVector.size() != 4) {	//인자가 3개여야 한다(라인, 인덱스, 삭제할 바이트)
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					try {
						int line = std::stoi(orderVector[1]);	//std::stoi를 사용해 문자열을 정수형으로
						int idx = std::stoi(orderVector[2]);	//std::stoi를 사용해 문자열을 정수형으로
						int lineLength = Singleton::instance()->getFile()[Singleton::instance()->getPage() + line].length();	//해당하는 라인의 범위
						int deByte = std::stoi(orderVector[3]);	//std::stoi를 사용해 문자열을 정수형으로

						if (line < 1 || line > 20) {
							Singleton::instance()->setMsg("Line의 범위는 1부터 20까지 입니다.");
						}
						else if (idx > lineLength || idx < 0) {
							Singleton::instance()->setMsg("index의 범위는 0부터 해당 라인 범위까지 입니다.");
						}
						else if (idx + deByte > lineLength) {
							Singleton::instance()->setMsg("삭제할 바이트가 없습니다.");
						}
						else {
							order_d(line, idx, deByte);
							Singleton::instance()->setMsg("");
						}
					}
					catch (std::invalid_argument const& e) {
						Singleton::instance()->setMsg("잘못된 입력입니다. 첫번째 인자와 두번째 인자와 세번째 인자로 정수형을 입력해주세요");
					}
				}
			}
			else if (orderVector[0] == "s") {	//찾기 명령일 때
				if (orderVector.size() != 2) {	//인자가 1개여야 한다(찾을 문자열)
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					if (!order_s(orderVector[1])) {
						Singleton::instance()->setMsg("파일에 존재하지 않는 단어입니다.");
					}
					else {
						Singleton::instance()->setMsg("");
					}
				}
			}
			else if (orderVector[0] == "c") {	//변경 명령일 때
				if (orderVector.size() != 3) {	//인자가 2개여야 한다(해당 문자열, 바꿀 문자열)
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					int oc = order_c(orderVector[1], orderVector[2]);
					if (oc == 0) {
						Singleton::instance()->setMsg("바꿀 단어가 파일에 존재하지 않습니다.");
					}
					else if (oc == 1) {
						Singleton::instance()->setMsg("라인의 최대 바이트를 넘어갑니다.");
					}
					else {
						Singleton::instance()->setMsg("");
					}
				}
			}
			else if (orderVector[0] == "t") {	//저장 후 종료 명령일 때
				if (orderVector.size() != 1) {	//인자가 있으면 안된다.
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					order_t();
					break;
				}
			}
			else if (orderVector[0] == "n") {	//다음 페이지 명령일 때
				if (orderVector.size() != 1) {	//인자가 있으면 안된다
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					if (Singleton::instance()->getPage() == Singleton::instance()->getFile().size() - 20) {
						Singleton::instance()->setMsg("이미 마지막 페이지 입니다.");	//마지막 페이지에서 n을 입력하면 콘솔메세지 업데이트
					}
					else {
						Singleton::instance()->setMsg("");
						order_n();
					}
				}
			}
			else if (orderVector[0] == "p") {	//이전 페이지 명령일 때
				if (orderVector.size() != 1) {	//인자가 있으면 안된다
					Singleton::instance()->setMsg("인자의 갯수가 맞지 않습니다.");
				}
				else {
					if (Singleton::instance()->getPage() == 0) {
						Singleton::instance()->setMsg("이미 첫번째 페이지 입니다.");	//처음 페이지에서 p를 입력하면 콘솔메세지 업데이트
					}
					else {
						Singleton::instance()->setMsg("");
						order_p();
					}
				}
			}
			else {	//명령이 i, d, s, c, t, n, p에 해당하지 않는다면
				Singleton::instance()->setMsg("잘못된 명령어 입니다.");	//콘솔메세지 업데이트
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