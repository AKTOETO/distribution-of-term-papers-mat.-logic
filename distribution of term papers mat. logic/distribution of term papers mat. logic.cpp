#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <tuple>
#include <ctime>
#include <iomanip>

constexpr auto VARIANTS = 95;

using namespace std;

class student_list
{
private:

	struct student
	{
		string name;
		string surname;
		string patronymic;

		enum class status_of_variant
		{
			chosen = 1,
			not_chosen
		}status = status_of_variant::chosen;
		int variant = -1;

		void random_var()
		{
			variant = 1 + rand() % (VARIANTS - 1);
		}

		friend ostream& operator<<(ostream& out, const student& stud)
		{
			string status =
				((int(stud.status) == 1) ? "chosen" : "not_chosen");

			out << stud.surname
				<< setw(15 - stud.surname.length()) << setfill(' ')
				<< " " << stud.name
				<< setw(15 - stud.name.length()) << setfill(' ')
				<< " " << stud.patronymic
				<< setw(15 - stud.patronymic.length()) << setfill(' ')
				<< " " << status
				<< setw(15 - status.length()) << setfill(' ')
				<< " " << stud.variant;
			return out;
		}
		friend bool operator<(const student& stud1, const student& stud2)
		{
			return tie(stud1.name, stud1.surname, stud1.patronymic)
				< tie(stud2.name, stud2.surname, stud2.patronymic);
		}
		friend bool operator==(const student& stud1, const student& stud2)
		{
			return tie(stud1.name, stud1.surname, stud1.patronymic,
				stud1.status, stud1.variant)
				== tie(stud2.name, stud2.surname, stud2.patronymic,
					stud2.status, stud2.variant);
		}
		friend bool operator!=(const student& stud1, const student& stud2)
		{
			return tie(stud1.name, stud1.surname, stud1.patronymic,
				stud1.status, stud1.variant)
				!= tie(stud2.name, stud2.surname, stud2.patronymic,
					stud2.status, stud2.variant);
		}
	};

	vector<student> m_student_list;

	string m_iofile = "data.txt";

public:
	student_list()
	{
		cout << "\tCREATE A STUDENT LIST\n";
	};

	~student_list()
	{
		cout << "\tDESTROY A STUDENT LIST\n";
	}

	friend ostream& operator<<(ostream& out, const student_list& data)
	{
		for (auto i = data.m_student_list.begin(); i != data.m_student_list.end(); i++)
		{
			out << *i << endl;
		}
		return out;
	}

	friend ostream& operator<<(ostream& out, const vector<student>& vec)
	{
		for (auto el : vec)
		{
			out << el << endl;
		}
		return out;
	}

	void delete_spaces(string& str, string delimiter = " ")
	{
		int pos = str.find_first_not_of(delimiter);
		str.erase(0, pos);
	}

	void read_student(string& str, student& stud, string delimiter = " ")
	{
		int pos = 0;
		string token;

		// read surname
		pos = str.find(delimiter);
		stud.surname = str.substr(0, pos);
		str.erase(0, pos + delimiter.length());
		delete_spaces(str);

		// read name
		pos = str.find(delimiter);
		stud.name = str.substr(0, pos);
		str.erase(0, pos + delimiter.length());
		delete_spaces(str);

		// read patronymic
		pos = str.find(delimiter);
		stud.patronymic = str.substr(0, pos);
		str.erase(0, pos + delimiter.length());
		delete_spaces(str);

		// read status
		pos = str.find(delimiter);
		string st = str.substr(0, pos);
		if (st == "chosen")
		{
			stud.status = student::status_of_variant::chosen;
		}
		else if (st == "not_chosen")
		{
			stud.status = student::status_of_variant::not_chosen;
		}
		else
		{
			cout << "ERROR NOT CORRECT STATUS: [" << st << "]\n";
			return;
		}
		str.erase(0, pos + delimiter.length());
		delete_spaces(str);

		// read variant
		pos = str.find(delimiter);
		stud.variant = stoi(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
		delete_spaces(str);
	}

	void read_data()
	{
		cout << "\tREADING...\n";
		ifstream fin(m_iofile);

		do
		{
			string str;
			int numb_of_var = 0;
			getline(fin, str);

			// ignoring end of string
			if (str == "\0")
				break;

			// reading full name
			student stud;
			read_student(str, stud);

			// adding to map full name && number of variant
			//data.emplace_hint(data.end(), stud, numb_of_var);
			m_student_list.push_back(stud);

		} while (!fin.eof());

		cout << m_student_list;

		fin.close();
		cout << "\tREADED!!!\n";
	}

	void write_data()
	{
		cout << "\tWRITING...\n";
		ofstream fout(m_iofile);

		for (auto el = m_student_list.begin(); el != m_student_list.end(); el++)
		{
			fout << *el;
			if (el != m_student_list.end() - 1)
				fout << endl;
		}
		cout << "\tWRITTEN!!!\n";
	}

	// to randomise variants
	void randomise_variant()
	{
		cout << "\tRANDOMISING...\n";
		for (auto el = m_student_list.begin(); el != m_student_list.end(); el++)
		{
			if (el->status == student::status_of_variant::not_chosen)
			{
				el->random_var();
				cout << *el << endl;
			}
		}
		cout << "\tRANDOMISED!!!\n";
	}

	// is the student unique with this variant
	vector<student> is_unique_student(student& stud)
	{
		vector<student> v_stud;
		for (auto el : m_student_list)
		{
			if (el != stud && el.variant == stud.variant)
				v_stud.push_back(el);
		}
		return v_stud;
	}

	// are the variants unique
	bool is_unique()
	{
		vector<int> variants;

		bool res = true;
		for (auto el : m_student_list)
		{
			if (find(variants.begin(), variants.end(), el.variant) == variants.end())
			{
				variants.push_back(el.variant);
				auto collis_stud = is_unique_student(el);
				if (collis_stud.size() != 0)
				{
					res = false;
					cout << "VARIANT COLLISION:\n";
					cout << '\t' << el << endl;

					for (auto elem : collis_stud)
					{
						cout << '\t' << elem << endl;
					}
					cout << endl;
				}
			}
		}
		return res;
	}

	// to make variants unique
	void make_unique()
	{
		if (!is_unique())
		{
			cout << "\tUNIFYING...\n";
			while (!is_unique())
			{
				randomise_variant();
			}
		}
		cout << "\tUNIFIED!!!\n";
	}

	/// <summary>
	/// function that can choose variant 
	/// </summary>
	/// <param name="surname"> - surname of student</param>
	/// <param name="variant"> - student variant</param>
	void choose_variant(string surname, int variant)
	{
		for (auto& el : m_student_list)
		{
			if (el.surname == surname)
			{
				el.variant = variant;
				el.status = student::status_of_variant::chosen;
				cout << "\t\tCHOSEN VARIANT: " << el.surname << " var: " << el.variant << endl;
				return;
			}
		}
	}
};

int main()
{
	srand(time(0));

	setlocale(LC_ALL, "ru");

	student_list sl;

	sl.read_data();
	//sl.choose_variant("Иващенко", 49);

	cout<<(sl.is_unique()?"\tUNIQUE!!!\n":"\tNOT UNIQUE!!!\n");
	//sl.randomise_variant();
	//sl.make_unique();
	sl.write_data();

	return 0;
}
