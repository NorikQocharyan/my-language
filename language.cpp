#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>


struct var_name
{
	std::string name;
	char type; // 'w' or 'l' or 'r' or 's'
	unsigned short index; // max 65535
	bool initialized; // 0 or 1
};
std::vector <std::string> s_v; //string_value
std::vector <long> l_v; //long_value
std::vector <double> d_v; //double_value
std::vector <bool> b_v; //bool_value
std::vector <var_name> v_n; //variable_name
std::vector <std::string> code;
std::vector <std::string> tokens;
std::vector <std::string> cycle;
std::string token;
std::string code_item;
std::string save;
int index = 0;
void create(std::vector <std::string>& tokens);
void math(std::vector <std::string>& tokens);
void unboxing(std::vector <std::string>& tokens);
void initialize(std::vector <std::string>& tokens);
void print(std::vector <std::string>& tokens);
void condition(std::vector <std::string>& tokens);
void who_are_u(std::string& code_item);
void memory();


int main()
{
	std::string path = "C:\\Users\\mrche\\OneDrive\\Desktop\\test_language.txt";
	std::ifstream fin;
	fin.open(path);
	if (fin.is_open()) {
		while (!fin.eof()) {
			std::string line;
			getline(fin, line);
			std::string tmp;
			for (int i = 0; i <= line.length(); i++) {

				if (line[i] != '\0') {
					tmp += line[i];
				}
				else {
					code.push_back(tmp);
					tmp = "";
				}
			}
		}
	}
	else {
		std::cout << "ERROR: file not found " << std::endl;
	}
	for (int i = 0; i < code.size(); i++)
	{
		std::string out = "";
		bool tmp = 0;
		for (int j = 0; j < code[i].size(); j++)
		{
			if (code[i][j] != ' ' && code[i][j] != '	') tmp = 1;
			if (tmp) out += code[i][j];
		}
		code[i] = out;
	}
	for (int i = 0; i < code.size(); i++) std::cout << code[i] << std::endl; //***

	std::cout << "************" << std::endl;

	for (; index < code.size(); index++)
	{
		who_are_u(code[index]);
	}


}

void create(std::vector <std::string>& tokens)
{

	//for (int j = 0; j < tokens.size(); j++) std::cout << tokens[j] << std::endl;
	for (int i = 0, turn = 0; i < tokens.size(); i++, turn++)
	{
		var_name var;
		var.type = tokens[0][0];
		i++;
		if (turn > 0)
			var.name = tokens[i - 1];
		else
			var.name = tokens[i];
		i++;

		if (tokens[i] == ">>") {
			i++;
		}
		else if (tokens[i] == "&" || tokens[i] == ":") {
			var.initialized = 0;
			v_n.push_back(var);
			continue;
		}
		var.initialized = 1;
		if (tokens[0] == "who") {
			long tmp = std::stol(tokens[i]);
			var.index = l_v.size();
			l_v.push_back(tmp);
		}
		else if (tokens[0] == "spe") {
			std::string tmp = tokens[i];
			var.index = s_v.size();
			s_v.push_back(tmp);
		}
		else if (tokens[0] == "rat") {
			double tmp = std::stod(tokens[i]);
			var.index = d_v.size();
			d_v.push_back(tmp);
		}
		else if (tokens[0] == "log") {
			bool tmp;
			if (tokens[i] == "real") tmp = 1;
			else if (tokens[i] == "unreal") tmp = 0;
			var.index = b_v.size();
			b_v.push_back(tmp);
		}
		i++;
		v_n.push_back(var);
	}
}
void math(std::vector <std::string>& tokens)
{
	for (int tmp = 0; tmp < tokens.size(); tmp++)
	{
		if (tokens[tmp] == "mul" || tokens[tmp] == "div")
		{
			char brand = tokens[tmp][0];
			double sos[3];
			sos[0] = std::stod(tokens[tmp - 1]);
			sos[1] = std::stod(tokens[tmp + 1]);
			tokens.erase(tokens.begin() + (tmp - 1));
			tokens.erase(tokens.begin() + (tmp));
			if (brand == 'm') sos[2] = sos[0] * sos[1];
			else sos[2] = sos[0] / sos[1];
			tmp--;
			tokens[tmp] = std::to_string(sos[2]);
		}
	}
	for (int tmp = 0; tmp < tokens.size(); tmp++)
	{
		if (tokens[tmp] == "plus" || tokens[tmp] == "minus")
		{
			char brand = tokens[tmp][0];
			double sos[3];
			sos[0] = std::stod(tokens[tmp - 1]);
			sos[1] = std::stod(tokens[tmp + 1]);
			tokens.erase(tokens.begin() + (tmp - 1));
			tokens.erase(tokens.begin() + (tmp));
			if (brand == 'p') sos[2] = sos[0] + sos[1];
			else sos[2] = sos[0] - sos[1];
			tmp--;
			tokens[tmp] = std::to_string(sos[2]);
		}
	}
}
void unboxing(std::vector <std::string>& tokens)
{
	for(int j = 1; j < tokens.size(); j++)
	{
		for (int i = 0; i < v_n.size(); i++)
		{
			if (tokens[j] == v_n[i].name)
			{
				if (v_n[i].type == 'w')
				{
					tokens[j] = std::to_string(l_v[v_n[i].index]);
				}
				else if (v_n[i].type == 'l')
				{
					tokens[j] = b_v[v_n[i].index];
				}
				else if (v_n[i].type == 'r')
				{
					tokens[j] = std::to_string(d_v[v_n[i].index]);
				}
				else if (v_n[i].type == 's')
				{
					tokens[j] = s_v[v_n[i].index];
				}
			}
		}
	}
}
void initialize(std::vector <std::string>& tokens)
{
	int j ;
	for (int i = 0; i < v_n.size(); i++) if (v_n[i].name == tokens[0]) j = i;
	if (v_n[j].initialized == 0)
	{
		if (v_n[j].type == 'w')
		{
			v_n[j].index = l_v.size();
			long tmp = std::stol(tokens[2]);
			l_v.push_back(tmp);
		}
		else if (v_n[j].type == 'l')
		{
			v_n[j].index = b_v.size();
			bool tmp;
			if (tokens[2] == "real") tmp = 1;
			else if (tokens[2] == "unreal") tmp = 0;
			b_v.push_back(tmp);
		}
		else if (v_n[j].type == 'r')
		{
			v_n[j].index = d_v.size();
			double tmp = std::stod(tokens[2]);
			d_v.push_back(tmp);
		}
		else if (v_n[j].type == 's')
		{
			v_n[j].index = s_v.size();
			std::string tmp = tokens[2];
			s_v.push_back(tmp);
		}
		v_n[j].initialized = 1;
	}
	else
	{
		if (v_n[j].type == 'w')
		{
			l_v[v_n[j].index] = std::stol(tokens[2]);
		}
		else if (v_n[j].type == 'l')
		{
			if (tokens[2] == "real") b_v[v_n[j].index] = 1;
			else if (tokens[2] == "unreal") b_v[v_n[j].index] = 0;
		}
		else if (v_n[j].type == 'r')
		{
			d_v[v_n[j].index] = std::stod(tokens[2]);
		}
		else if (v_n[j].type == 's')
		{
			s_v[v_n[j].index] = tokens[2];
		}

	}
}
void memory()
{
	for (int i = 0; i < v_n.size(); i++)
	{
		std::cout << "Name_" << v_n[i].name << " Type_" << v_n[i].type << " Initalized_" << v_n[i].initialized;
		if (v_n[i].initialized == 1)
		{
			std::cout << " Value_"; 
			if (v_n[i].type == 'w') std::cout << l_v[v_n[i].index] << std::endl;
			else if (v_n[i].type == 'l') std::cout << b_v[v_n[i].index] << std::endl;
			else if (v_n[i].type == 'r') std::cout << d_v[v_n[i].index] << std::endl;
			else if (v_n[i].type == 's') std::cout << s_v[v_n[i].index] << std::endl;
		}
		else std::cout << std::endl;
	}
}
void print(std::vector <std::string>& tokens)
{
	int i = 0;
	if (tokens[i] == "imprimer") i++; 
	if (tokens[i] == ":u") unboxing(tokens);
	else if (tokens[i] == ":m") math(tokens);
	else if (tokens[i] == ":u:m" || tokens[i] == ":m:u") { unboxing(tokens); math(tokens); }
	else
	{
		i++;
		for (; i < tokens.size() - 1; i++) std::cout << tokens[i] << " ";
	}
	i+=2;
	for (; i < tokens.size() - 1; i++)std::cout << tokens[i] << " ";
	std::cout << std::endl;
}
void condition(std::vector <std::string>& tokens)
{
	for (int tmp = 0; tmp < tokens.size(); tmp++)
	{
		if (tokens[tmp] == "same" || tokens[tmp] == "diff")
		{
			char brand = tokens[tmp][0];
			std::string sos[3];
			sos[0] = tokens[tmp - 1];
			sos[1] = tokens[tmp + 1];
			tokens.erase(tokens.begin() + (tmp - 1));
			tokens.erase(tokens.begin() + (tmp));
			if (brand == 's')
			{
				if (sos[0] == sos[1]) sos[2] = "1"; else sos[2] = "0";
			}
			else
			{
				if (sos[0] != sos[1]) sos[2] = "1"; else sos[2] = "0";
			}
			tmp--;
			tokens[tmp] = sos[2];
		}
	}
	for (int tmp = 0; tmp < tokens.size(); tmp++)
	{
		if (tokens[tmp] == "supra" || tokens[tmp] == "sore" || tokens[tmp] == "lower" || tokens[tmp] == "lore" || tokens[tmp] == "equ" || tokens[tmp] == "noequ")
		{
			std::string brand = tokens[tmp];
			double sos[3];
			sos[0] = std::stod(tokens[tmp - 1]);
			sos[1] = std::stod(tokens[tmp + 1]);
			tokens.erase(tokens.begin() + (tmp - 1));
			tokens.erase(tokens.begin() + (tmp));
			if (brand == "supra") sos[2] = sos[0] > sos[1];
			else if (brand == "sore") sos[2] = sos[0] >= sos[1];
			else if (brand == "lower") sos[2] = sos[0] < sos[1];
			else if (brand == "lore") sos[2] = sos[0] <= sos[1];
			else if (brand == "equ") sos[2] = sos[0] == sos[1];
			else if (brand == "noequ") sos[2] = sos[0] != sos[1];
			tmp--;
			tokens[tmp] = std::to_string(sos[2]);
		}
	}
	for (int tmp = 0; tmp < tokens.size(); tmp++)
	{
		if (tokens[tmp] == "and" || tokens[tmp] == "or")
		{
			char brand = tokens[tmp][0];
			double sos[3];
			sos[0] = std::stod(tokens[tmp - 1]);
			sos[1] = std::stod(tokens[tmp + 1]);
			tokens.erase(tokens.begin() + (tmp - 1));
			tokens.erase(tokens.begin() + (tmp));
			if (brand == 'a') sos[2] = sos[0] && sos[1];
			else sos[2] = sos[0] || sos[1];
			tmp--;
			tokens[tmp] = std::to_string(sos[2]);
		}
	}

}
void who_are_u(std::string& code_item)
{
	for (int i = 0; i <= code_item.length(); i++)
	{
		if (code_item[i] != ' ' && code_item[i] != '\0') token += code_item[i];
		else { tokens.push_back(token);	token = ""; }
	}
	if (tokens[0] == "who" || tokens[0] == "rat" || tokens[0] == "log" || tokens[0] == "spe")
	{
		unboxing(tokens);
		math(tokens);
		create(tokens);
	}
	else if (tokens[0] == "imprimer")
	{
		print(tokens);
	}
	else if (tokens[0] == "fourni")
	{
		int begin = index, finish = index+1, count = 0;
		bool tmp = 1;
		for (; tmp ; finish++)
		{
			if (code[finish] == "commencer") count++;
			else if (code[finish] == "cesser") count--;
			if (count == 0) tmp = 0;
		}
		finish--;
		index++;
		unboxing(tokens);
		math(tokens);
		condition(tokens);
		if ( std::stoi(tokens[2]) == 1)
		{
			for (; index < finish; index++)
			{
				tokens.clear();
				code_item = "";
				who_are_u(code[index]);
			}
		}
		index = finish;

	}
	else if (tokens[0] == "cycle")
	{
		int begin = index, finish = index + 1, count = 0;
		bool tmp = 1;
		for (; tmp; finish++)
		{
			if (code[finish] == "commencer") count++;
			else if (code[finish] == "cesser") count--;
			if (count == 0) tmp = 0;
		}
		finish--;
		for (; begin <= finish; begin++) cycle.push_back(code[begin]);
		begin = index;
		tmp = 0;
		unboxing(tokens);
		math(tokens);
		condition(tokens);
		if (std::stoi(tokens[2]) == 1) tmp = 1;
		while (tmp)
		{
			for (int j = index; j < finish; j++)
			{
				tokens.clear();
				code_item = "";
				who_are_u(code[j]);
			}

			tokens.clear();
			code_item = "";
			for (int i = begin, j = 0; i < finish; i++, j++)
			{
				code[i] = cycle[j];
			}
			code_item = code[index];
			for (int i = 0; i <= code_item.length(); i++)
			{
				if (code_item[i] != ' ' && code_item[i] != '\0') token += code_item[i];
				else { tokens.push_back(token);	token = ""; }
			}
			unboxing(tokens);
			math(tokens);
			condition(tokens);
			if (std::stoi(tokens[2]) == 1) tmp = 1;
			else tmp = 0;
		}
		index = finish;
	}
	else if (tokens[0] == "memory<>")
	{
		memory();
	}
	else
	{
		bool tmp = 0;
		for (int i = 0; i < v_n.size(); i++) if (tokens[0] == v_n[i].name) tmp = 1;
		if (tmp == 1)
		{
			unboxing(tokens);
			math(tokens);
			initialize(tokens);
		}
	}

	tokens.clear();
	code_item = "";
}