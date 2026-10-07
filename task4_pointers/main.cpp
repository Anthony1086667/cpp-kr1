#include <cstdlib>
#include <iomanip>
#include <iostream>

int main() {
  // Задача: журнал оценок потока. В потоке несколько групп, в каждой
  // своё число студентов, у всех студентов одинаковое число контрольных.
  // Программа считает средний балл каждого студента, каждой группы
  // и находит группу с лучшим средним.
  //
  // Зачем int***: данные трёхмерные, journal[группа][студент][контрольная].
  // Группы разного размера и размеры известны только при запуске,
  // поэтому int[A][B][C] не подходит. Нужен "массив массивов массивов":
  // int*** -> int** (студенты группы) -> int* (оценки студента) -> int.
  //
  // Зачем void*: весь журнал лежит в одном куске памяти из одного malloc.
  // В нём вперемешку объекты разных типов: сначала указатели на группы
  // (int**), потом указатели на студентов (int*), потом сами оценки (int).
  // Ни один конкретный тип не описывает такой буфер, поэтому он хранится
  // как void* - это просто байты, а нужные куски получаем приведением типов.
  // Без этого пришлось бы делать 1 + групп + студентов отдельных new
  // и столько же delete, и легко где-то потерять память.
  //
  // Формат ввода:
  //   G E          — число групп и число контрольных
  //   s1 ... sG    — размеры групп
  //   далее по строке на студента: E оценок

  int group_count = 0;
  int exam_count = 0;
  std::cin >> group_count >> exam_count;
  if (!std::cin || group_count <= 0 || exam_count <= 0) {
    std::cout << "error\n";
    return 1;
  }

  int* group_sizes = new int[group_count];
  int student_total = 0;
  for (int g = 0; g < group_count; ++g) {
    std::cin >> group_sizes[g];
    if (!std::cin || group_sizes[g] <= 0) {
      std::cout << "error\n";
      delete[] group_sizes;
      return 1;
    }
    student_total += group_sizes[g];
  }

  size_t groups_bytes = group_count * sizeof(int**);
  size_t students_bytes = student_total * sizeof(int*);
  size_t grades_bytes = student_total * exam_count * sizeof(int);

  void* memory = std::malloc(groups_bytes + students_bytes + grades_bytes);
  if (memory == nullptr) {
    std::cout << "error\n";
    delete[] group_sizes;
    return 1;
  }

  char* bytes = static_cast<char*>(memory);
  int*** journal = reinterpret_cast<int***>(bytes);
  int** student_rows = reinterpret_cast<int**>(bytes + groups_bytes);
  int* grades = reinterpret_cast<int*>(bytes + groups_bytes + students_bytes);

  int first_student = 0;
  for (int g = 0; g < group_count; ++g) {
    journal[g] = student_rows + first_student;
    for (int s = 0; s < group_sizes[g]; ++s) {
      journal[g][s] = grades + (first_student + s) * exam_count;
    }
    first_student += group_sizes[g];
  }

  bool input_ok = true;
  for (int g = 0; g < group_count && input_ok; ++g) {
    for (int s = 0; s < group_sizes[g] && input_ok; ++s) {
      for (int e = 0; e < exam_count; ++e) {
        if (!(std::cin >> journal[g][s][e])) {
          input_ok = false;
          break;
        }
      }
    }
  }

  if (input_ok) {
    std::cout << std::fixed << std::setprecision(2);
    int best_group = 0;
    double best_average = 0.0;

    for (int g = 0; g < group_count; ++g) {
      std::cout << "group " << g + 1 << ":\n";
      long long group_sum = 0;
      for (int s = 0; s < group_sizes[g]; ++s) {
        long long student_sum = 0;
        for (int e = 0; e < exam_count; ++e) {
          student_sum += journal[g][s][e];
        }
        group_sum += student_sum;
        std::cout << "  student " << s + 1 << ": "
                  << static_cast<double>(student_sum) / exam_count << '\n';
      }

      double group_average =
          static_cast<double>(group_sum) / (group_sizes[g] * exam_count);
      std::cout << "  group average: " << group_average << '\n';
      if (g == 0 || group_average > best_average) {
        best_group = g;
        best_average = group_average;
      }
    }
    std::cout << "best group: " << best_group + 1 << '\n';
  } else {
    std::cout << "error\n";
  }

  std::free(memory);
  delete[] group_sizes;
}
