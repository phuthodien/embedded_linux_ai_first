/*
 * student_grades.c - Demo breakpoint & step-by-step debugging
 *
 * Purpose:
 *   Simple program to practice GDB / VS Code debugging:
 *   - Set breakpoint at main(), calculate_average(), classify_grade()
 *   - Watch variables change as you step through
 *   - Inspect array, pointer, struct in debugger
 *
 * Contains an intentional bug (off-by-one) for students to find.
 *
 * Build:
 *   gcc -g -O0 -Wall -Wextra -std=c99 -o student_grades student_grades.c
 *
 * Debug with GDB:
 *   gdb ./student_grades
 *   (gdb) break main
 *   (gdb) run
 *   (gdb) next        # step over
 *   (gdb) step        # step into function
 *   (gdb) print scores # inspect array
 *   (gdb) continue
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 5
#define MAX_NAME_LEN 32

struct student {
	char name[MAX_NAME_LEN];
	int scores[3];		/* 3 subject scores */
	float average;
	char grade;		/* A, B, C, D, F */
};

/*
 * Calculate average of 3 scores.
 * TIP: set breakpoint here, watch 'sum' accumulate each iteration.
 */
float calculate_average(int scores[], int count)
{
	int sum = 0;

	/* BUG: <= should be < (off-by-one, reads past array) */
	for (int i = 0; i <= count; i++)
		sum += scores[i];

	return (float)sum / count;
}

/*
 * Classify grade based on average score.
 * TIP: set breakpoint here, inspect 'avg' value before each return.
 */
char classify_grade(float avg)
{
	if (avg >= 90.0f)
		return 'A';
	if (avg >= 80.0f)
		return 'B';
	if (avg >= 70.0f)
		return 'C';
	if (avg >= 60.0f)
		return 'D';
	return 'F';
}

/*
 * Print one student's info.
 * TIP: set breakpoint here, inspect struct fields via st->name, st->average.
 */
void print_student(const struct student *st)
{
	printf("%-15s | %3d %3d %3d | avg = %5.1f | grade = %c\n",
	       st->name,
	       st->scores[0], st->scores[1], st->scores[2],
	       st->average, st->grade);
}

int main(void)
{
	struct student class[MAX_STUDENTS] = {
		{ .name = "Nguyen Van A", .scores = {85, 90, 78} },
		{ .name = "Tran Thi B",   .scores = {92, 88, 95} },
		{ .name = "Le Van C",     .scores = {60, 55, 70} },
		{ .name = "Pham Thi D",   .scores = {45, 50, 40} },
		{ .name = "Hoang Van E",  .scores = {72, 68, 75} },
	};

	printf("=== Student Grade Report ===\n");
	printf("%-15s | Scores      | Average     | Grade\n", "Name");
	printf("------------------------------------------------------\n");

	/*
	 * TIP: set breakpoint inside this loop.
	 * Use "Watch" panel to monitor class[i].average changing.
	 * Use "Step Into (F11)" on calculate_average() to go inside.
	 */
	for (int i = 0; i < MAX_STUDENTS; i++) {
		class[i].average = calculate_average(class[i].scores, 3);
		class[i].grade = classify_grade(class[i].average);
		print_student(&class[i]);
	}

	printf("------------------------------------------------------\n");

	/* Count passing students (grade != F) */
	int pass_count = 0;

	for (int i = 0; i < MAX_STUDENTS; i++) {
		if (class[i].grade != 'F')
			pass_count++;
	}

	printf("Passed: %d / %d\n", pass_count, MAX_STUDENTS);

	return 0;
}
