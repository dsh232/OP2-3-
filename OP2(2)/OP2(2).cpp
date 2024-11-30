#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <iomanip> //для использования setw (устанавливает ширину поля для следующего вывода.)
#define DOT '.'
// Корректный ввод дробного значения. Проверка на ввод.
struct Step_Series {		//храним один член ряда 
	int n;
	double Sn, alpha_n, a;
};

struct Numerical_Series { //храним весь ряд
	double x;
	std::vector<Step_Series> steps_series;
};

struct Alpha {
	std::string alpha_str;
	bool is_n = true;
	double alpha;
};

//Методы. 
//cin.clear() - меняет состояние потока на good
//cin.peek() - смотрит следующий символ в потоке, но не извлекает.
//cin.get() - извлекает символ из потока (кушает)

double double_input()		//проверка на ввод.
{
	double input;
	while (!(std::cin >> input) //соответсвие введенного значения типу переменной
		|| std::cin.peek() != '\n')  //проверка на конец ввода
	{
		std::cin.clear();	//"лечим" поток. +состояние good
		while (std::cin.get() != '\n');
		std::cout << "Incorrect input. Please try again. " << std::endl;
	}
	return input;
}

// Корректный ввод на продолжить/остановить программу. Проверка на ввод.
char char_input()
{
	char input;
	while (!(std::cin >> input) || std::cin.peek() != '\n')
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Incorrect input. Please try again. " << std::endl;
	}
	return input;
}

Alpha alpha_input()		//проверка на ввод.
{
	Alpha alpha;
	std::string::size_type sz;

	while (!(std::cin >> alpha.alpha_str) || std::cin.peek() != '\n')
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Incorrect input. Please try again. " << std::endl;
	}
	int count = 0;
	int count_dot = 0;
	int count_sign = 0;
	bool flag = true;
	bool flag_dot = false;
	for (size_t i = 0; i < alpha.alpha_str.length(); i++)
	{
		if (alpha.alpha_str[i] == DOT)
		{
			count_dot++;
			flag_dot = true;
		}
		if (flag_dot) {
			count_sign++;
			alpha.is_n = false;
		}
		if (((!isdigit(alpha.alpha_str[i])) && (!flag_dot)) || (count_dot >= 2))
		{
			std::cout << "Incorrect input. Please try again." << std::endl;
			flag = false;
			break;
		}
	}
	if (!flag) {
		return alpha_input();
	}
	alpha.alpha = std::stod(alpha.alpha_str, &sz);

	return alpha;
}

// Подсчёт всех нужных значений
void calculation(int& n, double x, double& a, double& a_next, double& Sn, double& alpha_n) {

	a = a_next;	// Обновляем значение a
	Sn += a;	// Прибавляем значение a к частичной сумме Sn
	n++;
	a_next = a * (-x * x * (2 * (n)) * (2 * n - 1)) / ((2 * n + 2) * (2 * n + 1));		// Вычисляем следующее значение 
	alpha_n = std::abs(a_next / Sn);      // Вычисляем альфа_n как отношение |a_next| к |Sn|
}

int main()
{
	//setlocale(LC_ALL, "ru");
	std::vector<Numerical_Series> numerical_series_array;	//создаем вектор для Всех подсчитанных рядов

	bool is_need_next_run = false; //создание флага для повторного запуска программы
	while (is_need_next_run == false) { //цикл для повторного использования программы
		double x, alpha_n{}, a, a_next, Sn = 0, sum = 1;
		Alpha alpha;

		// Ввод параметра x запрашивается до тех пор, пока пользователь не введёт число соответсвующее требованиям.
		do
		{
			std::cout << "Enter parameter -1 < x < 1 : ";
			x = double_input();

		} while ((x > 1 || x < -1));

		// Ввод параметра alpha запрашивается до тех пор, пока пользователь не введёт положительное число.
		do
		{
			std::cout << "Enter the alpha parameter (Positive number): ";
			alpha = alpha_input();

		} while (alpha.alpha <= 0);

		Numerical_Series current_num_series;
		current_num_series.x = x;
		bool is_old_series = false;
		int index_list;

		for (index_list = 0; index_list < numerical_series_array.size(); index_list++) {
			Numerical_Series old_num_series = numerical_series_array.at(index_list);
			if (old_num_series.x == current_num_series.x) {
				current_num_series = old_num_series;
				is_old_series = true;
				break;
			}
		}

		a_next = x * x / 2;

		int n = 0;
		//Для красоты вывода
		std::cout << std::setw(5) << "n"
			<< std::setw(15) << "a"
			<< std::setw(15) << "Sn"
			<< std::setw(15) << "alpha_n" << "\n";

		do {
			Step_Series current_step;
			if (current_num_series.steps_series.size() > n) {	//проверяет входит ли элемент в старый ряд
				current_step = current_num_series.steps_series.at(n);
				n++;
				/*std::cout << "STAR:  ";*/
			}
			else {
				calculation(n, x, a, a_next, Sn, alpha_n); // Считаем данные
				current_step.n = n;
				current_step.a = a;
				current_step.Sn = Sn;
				current_step.alpha_n = alpha_n;
				current_num_series.steps_series.push_back(current_step);
			}


			// Выводим данные с установленной шириной
			std::cout << std::setw(5) << current_step.n
				<< std::setw(15) << current_step.a
				<< std::setw(15) << current_step.Sn
				<< std::setw(15) << current_step.alpha_n << std::endl;

		} while ((alpha.is_n && n < alpha.alpha) || (!alpha.is_n && alpha_n > alpha.alpha)); //проверяем шаги или проверяем точность

		if (is_old_series) numerical_series_array.at(index_list) = current_num_series;
		else numerical_series_array.push_back(current_num_series);

		bool is_need_processed = false;
		do {
			std::cout << "Do you want to continue? (Y/N):";
			char answer = char_input();
			if (answer == 'N') {
				std::cout << "Go home!!!" << std::endl;
				is_need_next_run = true;
				is_need_processed = true;
			}
			else if (answer == 'Y') {
				std::cout << "Continue? Of course!" << std::endl << std::endl;;
				is_need_processed = true;
			}

		} while (!is_need_processed);
	}
	numerical_series_array.clear();
}
