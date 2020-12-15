#include <iostream>
using namespace std;

template <class T>
struct data{
	T number;
};

template <class T>
class object{
public:
	data<T> info;
	object<T> *left;
	object<T> *right;

	static T sum;
	static int count;

public:
	object(){
		left = NULL;
		right = NULL;
	}
	object(data<T> i){
		info = i;
		left = NULL;
		right = NULL;
		object<T>::count++;
		sum += i.number;
	};

	static void Average();
	bool operator+=(data<T>& num);
	friend std::ostream& operator<< (std::ostream &out, const object<T> &obj);
	void operator-=(int num);
};

template <class T>
int object<T>::count = 0;
template <class T>
T object<T>::sum = 0;
template <class T>
object<T>* root = NULL;


template <class T, class T2>
class Tran {
	T new_value;
	T2* obj;
public:
	Tran(): new_value(0), obj(NULL){}
	T get(){
		return new_value;
	}
	bool BeginTrans_Add(T num);
	bool BeginTrans_Del(T num);
	void Commit();
	void DeleteTrans_Add();
	void DeleteTrans_Del();
};

template <class T, class T2>
bool Tran<T,T2>::BeginTrans_Add(T num) {
	new_value = num;
	data<T> d_root = {num};
	object<T> o_root;
	if (o_root += d_root){
		return true;
	}
	return false;
}

template <class T, class T2>
bool Tran<T,T2>::BeginTrans_Del(T num) {
	new_value = num;
	object<T>* objec = root<T>;
	object<T>* temp = objec;

	while(objec!= NULL && objec->info.number != num){
		temp = objec;
		if(objec->info.number > num){
			objec = objec->left;
		} else if(objec->info.number < num){
			objec = objec->right;
		}
	}

	if(objec == NULL){
		cout << endl << "There's no such a number to delete!" << endl;
		return false;
	} else{
		objec = temp;
	}

	if(objec->info.number > num){
		obj = objec->left;
		objec->left = NULL;
	} else{
		obj = objec->right;
		objec->right = NULL;
	}
	return true;
}

template <class T, class T2>
void Tran<T,T2>::Commit () {
	new_value = 0;
	obj = NULL;
}

template <class T, class T2>
void Tran<T,T2>::DeleteTrans_Add(){
	object<T> o_root;
	o_root -= new_value;
	new_value = 0;
	obj = NULL;
}

template <class T, class T2>
void Tran<T,T2>::DeleteTrans_Del(){
	object<T>* objec = root<T>;
	object<T>* temp = objec;
	int num = obj->info.number;
	while(objec!= NULL && objec->info.number != num){
		temp = objec;
		if(objec->info.number > num){
			objec = objec->left;
		} else if(objec->info.number < num){
			objec = objec->right;
		}
	}
	objec = temp;

	if(objec->info.number > num){
		objec->left = obj;
	} else{
		objec->right = obj;
	}

	new_value = 0;
	obj = NULL;
}

template <class T>
object<T>* Find_place(object<T>* to_add, object<T>* temp){
	object<T>* temp2 = temp;

	while(temp != NULL){
		temp2 = temp;
		if(temp->info.number > to_add->info.number){
			temp = temp->left;
		} else if (temp->info.number < to_add->info.number){
			temp = temp->right;
		} else {
			break;
		}
	}
	return temp2;
}

template <class T>
bool object<T>::operator +=(data<T>& num){
	object* new_obj = new object(num);

	if(root<T> == NULL){
		root<T> = new_obj;
	} else {
		object* place = Find_place(new_obj, root<T>);

		if(place->info.number > new_obj->info.number){
			place->left = new_obj;
		} else if (place->info.number < new_obj->info.number){
			place->right = new_obj;
		} else {
			cout  << endl << "The number is already in the tree" << endl;
			return false;
		}
	}
	return true;
}

template <class T>
void Del_object(object<T>* obj){
	if(obj == NULL){
		return;
	}

	Del_object(obj->left);
	Del_object(obj->right);
	object<T>::count--;
	object<T>::sum -= obj->info.number;
	delete obj;
}

template <class T>
void object<T>::operator-=(int num){
	if(root<T> == NULL){
		cout << "Tree is empty!";
		return;
	}
	if(root<T>->info.number == num){
		Del_object(root<T>->left);
		Del_object(root<T>->right);
		root<T> = NULL;
		object<T>::sum = 0;
		object<T>::count = 0;
		cout << endl << "Whole tree is deleted" << endl;
	} else{
		object<T>* obj = root<T>;
		object<T>* temp = obj;

		while(obj!= NULL && obj->info.number != num){
			temp = obj;
			if(obj->info.number > num){
				obj = obj->left;
			} else if(obj->info.number < num){
				obj = obj->right;
			}
		}

		if(obj == NULL){
			cout << endl << "There's no such a number to delete!" << endl;
			return;
		} else{
			obj = temp;
		}

		if(obj->info.number > num){
			Del_object(obj->left);
			obj->left = NULL;
		} else{
			Del_object(obj->right);
			obj->right = NULL;
		}

		cout << endl << "Object deleted" << endl;
	}
}

template <class T>
void Show_obj(object<T>* obj){
	if(obj == NULL){
		return;
	}

	cout << ", " << obj->info.number;

	Show_obj(obj->left);
	Show_obj(obj->right);
}

template <class T>
std::ostream& operator<< (std::ostream &out, object<T> &obj){
	if(root<T> == NULL){
		cout << "Tree is empty!";
		return out;
	}

	cout << endl << endl << "Hi! I'm a tree!" << endl;
	cout << root<T>->info.number;

	Show_obj(root<T>->left);
	Show_obj(root<T>->right);
	return out;
}
template <class T>
void object<T>::Average(){
	if(root<T> == NULL){
		cout << "Tree is empty";
	} else{
		cout << endl << endl << "The average = " << (double)object::sum/object::count << endl;
	}
}

bool IsNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
string IsCorrect(string request){
	string item;
	do{
		cout << request;
		getline(cin, item);
		while(IsNumber(item) == false){
				cout << "\nINCORRECT" << endl;
				cout << request;
				getline(cin, item);
		}
	}while(stoi(item) < 1);
	return item;
}

int main(){
	cout << "The generated numbers: 50";
	data<int> d_root = {50};
	object<int> o_root;
	o_root += d_root;

	for(int i = 0; i < 10; i++){
		int rand_num = rand() % 100;

		data<int> d_el = {rand_num};
		object<int> o_el;
		o_el += d_el;
		cout << ", " << rand_num ;
	}
	cout << o_root;

	string new_num, del_num, answer;
	int choice;

	Tran<int, object<int> > tr;

	do{
		cout << endl << endl << "1 - for add; \n2 - for delete; \n3 - for average; \n4 - for print; \n5 - for exit..." << endl;
		answer = IsCorrect("Your choice: ");

		choice = stoi(answer);

    	switch(choice){
		case 1:
			cout << "Adding the element.";
			new_num = IsCorrect("Your number = ");

			cout << "\n\nObject state before transaction ";
			cout << o_root;

			if(tr.BeginTrans_Add(stoi(new_num))){
				cout << "\n\nObject state after transaction ";
				cout << o_root;

				do{
					cout << "\n\nCOMMIT? (Y/N): ";
					getline(cin, answer);
				}while(answer != "y" && answer != "Y" && answer != "n" && answer != "N");

				if(answer != "y" && answer != "Y"){
					tr.DeleteTrans_Add();
					cout << "Transaction cancelled.";
					cout << o_root;
				} else{
					tr.Commit();
					cout << "Transaction commited.";
					cout << o_root;
				}
			}
			break;
		case 2:
			cout << "Deleting the element.";
			del_num = IsCorrect("Your number = ");

			cout << "\n\nObject state before transaction ";
			cout << o_root;

			if(tr.BeginTrans_Del(stoi(del_num))){
				cout << "\n\nObject state after transaction ";
				cout << o_root;

				do{
					cout << "\n\nCOMMIT? (Y/N): ";
					getline(cin, answer);
				}while(answer != "y" && answer != "Y" && answer != "n" && answer != "N");

				if(answer != "y" && answer != "Y"){
					tr.DeleteTrans_Del();
					cout << "Transaction cancelled.";
					cout << o_root;
				} else{
					tr.Commit();
					cout << "Transaction commited.";
					cout << o_root;
				}
			}
			break;
		case 3:
			object<int>::Average();
			break;
		case 4:
			cout << o_root;
			break;
		case 5:
			break;
		}
	}while(choice != 5);

	cout << "Goodbye";
	return 0;
}
