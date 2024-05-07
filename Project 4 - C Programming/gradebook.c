#include "gradebook.h"
#include <string.h>

/*
 * Name: Hieu Nguyen
 */

struct Gradebook gradebook;

/**
 * Adds a new student to the gradebook and sets all the student's grades to 0.
 *
 * Updates assignment_averages and course_average based on the new grades.
 *
 * @param name The name of the student.
 * @param gtid The GTID of the student.
 * @param year The year of the student.
 * @param major The major of the student.
 * @return SUCCESS if the student is added, otherwise ERROR if the student can't
 * be added (duplicate name / GTID, no space in gradebook, invalid major).
 */
int add_student(char *name, int gtid, int year, char *major) {
  // TODO: Implement this function
  // Check if name or major is included to be added
  if (!name || !major) {
    return ERROR;
  }

  // Check if there is space in the gradebook
  if (gradebook.size >= MAX_ENTRIES) {
    return ERROR;
  }

  // Check if length of name exceeds length limit
  if (strlen(name) + 1 > MAX_NAME_LENGTH) {
    return ERROR;
  }

  struct Student newStudent;
  for (int j = 0; j < (int) strlen(name); j++) {
    newStudent.name[j] = name[j];
  }
  newStudent.gtid = gtid;
  newStudent.year = year;

  // Check for valid major
  if (strcmp(major,"CS") == 0) {
    newStudent.major = CS;
  } else if (strcmp(major, "CE") == 0) {
    newStudent.major = CE;
  } else if (strcmp(major, "EE") == 0) {
    newStudent.major = EE;
  } else if (strcmp(major, "IE") == 0) {
    newStudent.major = IE;
  } else {
    return ERROR;
  }

  // Check for duplicate name or GTID
  for (int i = 0; i < gradebook.size; i++) {
        if (strcmp(gradebook.entries[i].student.name, name) == 0 || gradebook.entries[i].student.gtid == gtid) {
            return ERROR;
        }
  }

  // Add new student
  struct GradebookEntry newBookEntry;
  newBookEntry.student = newStudent;
  newBookEntry.average = 0;

  gradebook.entries[gradebook.size] = newBookEntry;
  gradebook.size++;
  return SUCCESS;
}


/**
 * Updates the grade of a specific assignment for a student and updates that
 * student's average grade.
 * 
 * Ensure that the overall course averages are still up-to-date after these grade updates.
 *
 * @param name The name of the student.
 * @param assignmentType The type of assignment.
 * @param newGrade The new grade.
 * @return SUCCESS if the grade is updated, otherwise ERROR if the grade isn't (student not found).
 */
int update_grade(char *name, enum Assignment assignment_type, double new_grade) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    return ERROR;
  }

  int index = search_student(name);
  if (index == ERROR) {
    return ERROR; // if student is not found
  }

  gradebook.entries[index].grades[assignment_type] = new_grade;

  calculate_average(&gradebook.entries[index]);
  for (int i = 0 ; i < NUM_ASSIGNMENTS; ++i) {
    double total = 0.0;
    for (int j = 0; j < gradebook.size; ++j) {
      total += gradebook.entries[i].grades[i];
    }
    gradebook.assignment_averages[i] = total / gradebook.size;
  }

  calculate_course_average();
  return SUCCESS;
}

/**
 * Adds a new student to the gradebook and initializes each of the student's
 * grades with the grades passed in.
 *
 * Additionally, will update the overall assignment_averages and course_average
 * based on the new added student.
 *
 * @param name The name of the student.
 * @param gtid The GTID of the student.
 * @param year The year of the student.
 * @param major The major of the student.
 * @param grades An array of grades for the student.
 * @return SUCCESS if the student is added and the averages updated, otherwise ERROR if the student can't
 * be added (duplicate name / GTID, no space in gradebook, invalid major).
 */
int add_student_with_grades(char *name, int gtid, int year, char *major,
                            double *grades) {
  // TODO: Implement this function
  // Check if name, major, or grade is included to be added
  if (!name || !major || !grades) {
    return ERROR;
  }

  // Check if there is space in the gradebook
  if (gradebook.size >= MAX_ENTRIES) {
    return ERROR;
  }

  // Check if length of name exceeds length limit
  if (strlen(name) + 1 > MAX_NAME_LENGTH) {
    return ERROR;
  }

  struct Student newStudent;
  for (int j = 0; j < (int) strlen(name); j++) {
    newStudent.name[j] = name[j];
  }

  newStudent.gtid = gtid;
  newStudent.year = year;

  // Check for valid major
  if (strcmp(major, "CS") == 0) {
    newStudent.major = CS;
  } else if (strcmp(major, "CE") == 0) {
    newStudent.major = CE;
  } else if (strcmp(major, "EE") == 0) {
    newStudent.major = EE;
  } else if (strcmp(major, "IE") == 0) {
    newStudent.major = IE;
  } else {
    return ERROR;
  }

  // Check for duplicate name or GTID
  for (int i = 0; i < gradebook.size; i++) {
    if (strcmp(gradebook.entries[i].student.name, name) == 0 || gradebook.entries[i].student.gtid == gtid) {
      return ERROR;
    }
  }

  // Add the new student
  struct GradebookEntry newBookEntry;
  newBookEntry.student = newStudent;
  newBookEntry.average = 0;

  // Copy grades to new entry
  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    newBookEntry.grades[i] = grades[i];
  }

  // Calculate the average
  calculate_average(&newBookEntry);

  // Add the new student entry to gradebook
  gradebook.entries[gradebook.size] = newBookEntry;
  gradebook.size++;

  // Update overall averages
  calculate_course_average();

  return SUCCESS;
}

/**
 * Calculates the average grade for a specific gradebook entry and updates the
 * struct as appropriate.
 *
 * @param entry The gradebook entry for which to recalculate the average.
 * @return SUCCESS if the average is updated, ERROR if pointer is NULL
 */
int calculate_average(struct GradebookEntry *entry) {
  // TODO: Implement this function
  if (!entry) {
    return ERROR;
  }

  double sum = 0;
  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    sum += entry->grades[i] * gradebook.weights[i];
  }

  entry->average = sum;
  return SUCCESS;
}

/**
 * Calculates and update the overall course average and assignment averages. 
 * The average should be calculated by taking the averages of the student's 
 * averages, NOT the assignment averages.
 *
 * If the gradebook is empty, set the course and assignment averages to 0
 * and return ERROR.
 * 
 * @return SUCCESS if the averages are calculated properly, ERROR if gradebook
 * is empty
 */
int calculate_course_average(void) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    gradebook.course_average = 0;
    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
      gradebook.assignment_averages[i] = 0;
    }
    return ERROR;
  }

  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    gradebook.assignment_averages[i] = 0;
  }

  double sum = 0;
  for (int i = 0; i < gradebook.size; i++) {
    sum += gradebook.entries[i].average;
    for (int j = 0; j < NUM_ASSIGNMENTS; j++) {
      gradebook.assignment_averages[j] += gradebook.entries[i].grades[j];
    }
  }

  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    gradebook.assignment_averages[i] /= gradebook.size;
  }

  gradebook.course_average = sum / gradebook.size;
  return SUCCESS;
}

/**
 * Searches for a student in the gradebook by name.
 *
 * @param name The name of the student.
 * @return The index of the student in the gradebook, or ERROR if not found.
 */
int search_student(char *name) {
  // TODO: Implement this function
  if (!name) {
    return ERROR;
  }

  for (int i = 0; i < gradebook.size; i++) {
    if (strcmp(gradebook.entries[i].student.name, name) == 0) {
      return i; // Found the student at index i
    }
  }
  return ERROR;
}

/**
 * Remove a student from the gradebook while maintaining the ordering of the gradebook.
 *
 * Additionally, update the overall assignment_averages and course_average
 * based on the removed student and decrement the size of gradebook.
 *
 * If the gradebook is empty afterwards, SUCCESS should still be returned and
 * averages should be set to 0.
 *
 * @param name The name of the student to be withdrawn.
 * @return SUCCESS if the student is successfully removed, otherwise ERROR if
 * the student isn't found.
 */
int withdraw_student(char *name) {
  // TODO: Implement this function
  int index = search_student(name);
  if (index == ERROR) {
    return ERROR; // Student not found
  }

  // Update assignment averages and course average
  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    gradebook.assignment_averages[i] -= gradebook.entries[index].grades[i] / gradebook.size;
  }

  // Remove the student and update averages
  for (int i = index; i < gradebook.size - 1; i++) {
    gradebook.entries[i] = gradebook.entries[i + 1];
  }

  gradebook.size--;

  calculate_course_average();
  return SUCCESS;
}

/**
 * Populate the provided array with the GTIDs of the 5 students with the highest
 * grades. The GTIDs should be placed in descending order of averages. 
 * 
 * If unable to populate the full array (less than 5 students in gradebook), 
 * fill in the remaining values with INVALID_GTID.
 *
 * @param gtids An array to store the top five gtids.
 * @return SUCCESS if gtids are found, otherwise ERROR if gradebook is empty
 */
int top_five_gtid(int *gtids) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    return ERROR;
  }

  // Create an array of indices
  int indices[MAX_ENTRIES];
  for (int i = 0; i < gradebook.size; i++) {
    indices[i] = i;
  }

  // Bubble sorting to find top five gtids
  for (int i = 0; i < gradebook.size - 1; i++) {
    for (int j = 0; j < gradebook.size - i - 1; j++) {
      if (gradebook.entries[indices[j]].average < gradebook.entries[indices[j + 1]].average) {
        int temp = indices[j];
        indices[j] = indices[j + 1];
        indices[j + 1] = temp;
      }
    }
  }

  // Populate gtids with the top five GTIDs
  for (int i = 0; i < 5; i++) {
    gtids[i] = (i < gradebook.size) ? gradebook.entries[indices[i]].student.gtid : INVALID_GTID;
  }
  return SUCCESS;
}

/**
 * Sorts the gradebook entries by name in alphabetical order (First, Last).
 *
 * @return SUCCESS if names are sorted, ERROR is gradebook is empty.
 */
int sort_name(void) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    return ERROR; // Gradebook is empty
  }

  // Bubble sorting
  for (int i = 0; i < gradebook.size - 1; i++) {
    for (int j = 0; j < gradebook.size - i - 1; j++) {
      if (strcmp(gradebook.entries[j].student.name, gradebook.entries[j + 1].student.name) > 0) {
        struct GradebookEntry temp = gradebook.entries[j];
        gradebook.entries[j] = gradebook.entries[j + 1];
        gradebook.entries[j + 1] = temp;
      }
    }
  }
  return SUCCESS;
}

/**
 * Sorts the gradebook entries by average grades in descending order.
 *
 * @return SUCCESS if entries are sorted, ERROR if gradebook is empty.
 */
int sort_averages(void) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    return ERROR; // Gradebook is empty
  }

  // Bubble sorting
  for (int i = 0; i < gradebook.size - 1; i++) {
    for (int j = 0; j < gradebook.size - i - 1; j++) {
      if (gradebook.entries[j].average < gradebook.entries[j + 1].average) {
        struct GradebookEntry temp = gradebook.entries[j];
        gradebook.entries[j] = gradebook.entries[j + 1];
        gradebook.entries[j + 1] = temp;
      }
    }
  }

  return SUCCESS;
}

/**
 * Prints the entire gradebook in the format
 * student_name,major,grade1,grade2,...,student_average\n
 * 
 * Overall Averages:
 * grade1_average,grade2_average,...,course_average\n
 * 
 * Note 1: The '\n' shouldn’t print, just represents the newline for this example.
 * Note 2: There is an empty line above the “Overall Averages:” line.
 * 
 * All of the floats that you print must be manually rounded to 2 decimal places.
 *
 * @return SUCCESS if gradebook is printed, ERROR if gradebook is empty.
 */
int print_gradebook(void) {
  // TODO: Implement this function
  if (gradebook.size == 0) {
    return ERROR;
  }
  
  // Print each student entry
  for (int i = 0; i < gradebook.size; i++) {
    printf("%s,%s",
           gradebook.entries[i].student.name,
           (gradebook.entries[i].student.major == CS) ? "CS" :
           (gradebook.entries[i].student.major == CE) ? "CE" :
           (gradebook.entries[i].student.major == EE) ? "EE" :
           (gradebook.entries[i].student.major == IE) ? "IE" : "Unknown");

    // Print grades
    for (int j = 0; j < NUM_ASSIGNMENTS; j++) {
      printf(",%.2f", gradebook.entries[i].grades[j]);
    }

    // Print average
    printf(",%.2f\n", gradebook.entries[i].average);
  }

  // Print overall averages
  printf("\nOverall Averages:\n");
  for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
    printf("%.2f,", gradebook.assignment_averages[i]);
  }

  // Print course average
  printf("%.2f\n", gradebook.course_average);

  return SUCCESS;
}
