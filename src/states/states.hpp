/*Header file for all of the states.*/
#ifndef __STATES_H__
#define __STATES_H__

#include "state_interface.hpp"
#include <string>
#include <fstream>
#include "file_io.hpp"

inline constexpr int NUMBER_OF_STATES = 39;

class StateMainMenu : public State
{
public:
	StateMainMenu();
	~StateMainMenu();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		STUDENTS,
		COURSES,
		COURSE_REGISTRATION,
		REPORTS,
		EXIT
	};

	menu_t current_menu;
};

class StateStudentsMenu : public State
{
public:
	StateStudentsMenu();
	~StateStudentsMenu();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		ADD_STUDENT,
		REMOVE_STUDENT,
		SEARCH_STUDENT,
		BACK
	};

	menu_t current_menu;
};

class StateAddStudent : public State
{
public:
	StateAddStudent();
	~StateAddStudent();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		FIRST_NAME,
		LAST_NAME,
		ID,
		CANCEL,
		OKAY
	};

	menu_t current_menu;
	int first_name_cursor;
	int last_name_cursor;
	int id_cursor;
	std::string first_name;
	std::string last_name;
	std::string id;
	bool invalid_input;
};

class StateNotificationStudentAddSuccess : public State
{
public:
	StateNotificationStudentAddSuccess();
	~StateNotificationStudentAddSuccess();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY
	};

	menu_t current_menu;
};

class StateNotificationStudentAddFail : public State
{
public:
	StateNotificationStudentAddFail();
	~StateNotificationStudentAddFail();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateNotificationStudentAddDuplicate : public State
{
public:
	StateNotificationStudentAddDuplicate();
	~StateNotificationStudentAddDuplicate();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateSearchStudent : public State
{
public:
	StateSearchStudent();
	~StateSearchStudent();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		STUDENT_ID,
		CANCEL,
		SEARCH
	};

	menu_t current_menu;
	int id_cursor;
	std::string first_name;
	std::string last_name;
	std::string id;
};

class StateRemoveStudent : public State
{
public:
	StateRemoveStudent();
	~StateRemoveStudent();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();
	bool remove;

private:
	size_t current_index;
	int current_list_page;
	int total_list_pages;
	int last_name_cols;
	int first_name_cols;
	int id_cols;
	size_t list_size;
	size_t page_size;
	bool done_calculations;
};

class StateNotificationRemoveStudentPopup : public State
{
public:
	StateNotificationRemoveStudentPopup();
	~StateNotificationRemoveStudentPopup();
	state_return_t stateFunction(int input);

private:
	enum class menu_t
	{
		OKAY,
		CANCEL
	};

	menu_t current_menu;
};

class StateCoursesMenu : public State
{
public:
	StateCoursesMenu();
	~StateCoursesMenu();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		ADD_THEORETICAL_COURSE,
		ADD_PRACTICAL_COURSE,
		REMOVE_COURSE,
		SEARCH_COURSE,
		BACK
	};

	menu_t current_menu;
};

class StateAddTheoreticalCourse : public State
{
public:
	StateAddTheoreticalCourse();
	~StateAddTheoreticalCourse();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		COURSE_NAME,
		ID,
		UNITS,
		LESSON_PLAN,
		CANCEL,
		OKAY
	};

	menu_t current_menu;
	int course_name_cursor;
	int units_cursor;
	int id_cursor;
	int lesson_plan_cursor;
	int units;
	std::string course_name;
	std::string lesson_plan;
	std::string id;
	std::string units_string;
	bool invalid_input;
};

class StateAddPracticalCourse : public State
{
public:
	StateAddPracticalCourse();
	~StateAddPracticalCourse();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		COURSE_NAME,
		ID,
		UNITS,
		PROJECT_PLAN,
		CANCEL,
		OKAY
	};

	menu_t current_menu;

	int course_name_cursor;
	int units_cursor;
	int id_cursor;
	int project_plan_cursor;
	int units;
	std::string course_name;
	std::string project_plan;
	std::string id;
	std::string units_string;
	bool invalid_input;
};

class StateNotificationCourseAddSuccess : public State
{
public:
	StateNotificationCourseAddSuccess();
	~StateNotificationCourseAddSuccess();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY
	};

	menu_t current_menu;
};

class StateNotificationPracticalCourseAddFail : public State
{
public:
	StateNotificationPracticalCourseAddFail();
	~StateNotificationPracticalCourseAddFail();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateNotificationPracticalCourseAddDuplicate : public State
{
public:
	StateNotificationPracticalCourseAddDuplicate();
	~StateNotificationPracticalCourseAddDuplicate();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateNotificationTheoreticalCourseAddFail : public State
{
public:
	StateNotificationTheoreticalCourseAddFail();
	~StateNotificationTheoreticalCourseAddFail();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateNotificationTheoreticalCourseAddDuplicate : public State
{
public:
	StateNotificationTheoreticalCourseAddDuplicate();
	~StateNotificationTheoreticalCourseAddDuplicate();
	void inputHandler(int input);
	void mainWindowHandler();
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateSearchCourse : public State
{
public:
	StateSearchCourse();
	~StateSearchCourse();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		COURSE_ID,
		CANCEL,
		SEARCH
	};

	menu_t current_menu;
	int id_cursor;
	std::string id;
	std::string course_name;
	std::string course_plan;
	int course_unit;
};

class StateRemoveCourse : public State
{
public:
	StateRemoveCourse();
	~StateRemoveCourse();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();
	bool remove;

private:
	size_t current_index;
	int current_list_page;
	int total_list_pages;
	int name_cols;
	int id_cols;
	int plan_cols;
	size_t list_size;
	size_t page_size;
	bool done_calculations;
};

class StateNotificationRemoveCoursePopup : public State
{
public:
	StateNotificationRemoveCoursePopup();
	~StateNotificationRemoveCoursePopup();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY,
		CANCEL
	};

	menu_t current_menu;
};

class StateRegisterationMenu : public State
{
public:
	StateRegisterationMenu();
	~StateRegisterationMenu();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		REGISTER,
		UNREGISTER,
		ENTER_GRADES,
		BACK
	};

	menu_t current_menu;
};

class StateCourseRegister : public State
{
public:
	StateCourseRegister();
	~StateCourseRegister();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t student_current_index;
	size_t course_current_index;
	int student_current_list_page;
	int student_total_list_pages;
	int course_current_list_page;
	int course_total_list_pages;
	int student_last_name_cols;
	int student_first_name_cols;
	int student_id_cols;
	int course_name_cols;
	int course_plan_cols;
	int course_id_cols;
	size_t student_list_size;
	size_t course_list_size;
	size_t page_size;
	bool done_calculations;
	bool student_selected;
};

class StateCourseUnregister : public State
{
public:
	StateCourseUnregister();
	~StateCourseUnregister();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t student_current_index;
	size_t course_current_index;
	int student_current_list_page;
	int student_total_list_pages;
	int course_current_list_page;
	int course_total_list_pages;
	int student_last_name_cols;
	int student_first_name_cols;
	int student_id_cols;
	int course_name_cols;
	int course_plan_cols;
	int course_id_cols;
	size_t student_list_size;
	size_t course_list_size;
	size_t page_size;
	bool done_calculations;
	bool student_selected;
};

class StateNotificationUnregisterSuccess : public State
{
public:
	StateNotificationUnregisterSuccess();
	~StateNotificationUnregisterSuccess();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY
	};

	menu_t current_menu;
};

class StateGradeEntry : public State
{
public:
	StateGradeEntry();
	~StateGradeEntry();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		GRADE,
		CANCEL,
		OKAY
	};
	menu_t current_menu;
	size_t student_current_index;
	size_t course_current_index;
	size_t student_list_size;
	size_t course_list_size;
	size_t page_size;
	int student_current_list_page;
	int student_total_list_pages;
	int course_current_list_page;
	int course_total_list_pages;
	int student_last_name_cols;
	int student_first_name_cols;
	int student_id_cols;
	int course_name_cols;
	int course_plan_cols;
	int course_id_cols;
	int grade_cursor;
	bool done_calculations;
	bool student_selected;
	bool grade_entry_page;
	std::string grade_string;
};

class StateNotificationGradeEntrySuccess : public State
{
public:
	StateNotificationGradeEntrySuccess();
	~StateNotificationGradeEntrySuccess();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY
	};

	menu_t current_menu;
};

class StateNotificationGradeEntryFail : public State
{
public:
	StateNotificationGradeEntryFail();
	~StateNotificationGradeEntryFail();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateNotificationRegisterSuccess : public State
{
public:
	StateNotificationRegisterSuccess();
	~StateNotificationRegisterSuccess();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		OKAY
	};

	menu_t current_menu;
};

class StateNotificationRegisterFail : public State
{
public:
	StateNotificationRegisterFail();
	~StateNotificationRegisterFail();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		BACK
	};

	menu_t current_menu;
};

class StateReportsMenu : public State
{
public:
	StateReportsMenu();
	~StateReportsMenu();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	enum class menu_t
	{
		STUDENT_LIST,
		COURSE_LIST,
		COURSE_GRADES,
		REGISTERED_STUDENTS,
		BACK
	};

	menu_t current_menu;
};

class StateStudentsList : public State
{
public:
	StateStudentsList();
	~StateStudentsList();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t current_index;
	int current_list_page;
	int total_list_pages;
	int last_name_cols;
	int first_name_cols;
	int id_cols;
	size_t list_size;
	size_t page_size;
	bool done_calculations;
};

class StateCoursesList : public State
{
public:
	StateCoursesList();
	~StateCoursesList();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t current_index;
	int current_list_page;
	int total_list_pages;
	int name_cols;
	int plan_cols;
	int id_cols;
	size_t list_size;
	size_t page_size;
	bool done_calculations;
};

class StateCourseGrades : public State
{
public:
	StateCourseGrades();
	~StateCourseGrades();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t course_current_index;
	int course_current_list_page;
	int course_total_list_pages;
	int course_name_cols;
	int course_plan_cols;
	int course_id_cols;
	size_t course_list_size;

	size_t page_size;

	size_t student_current_index;
	int student_current_list_page;
	int student_total_list_pages;
	int student_first_name_cols;
	int student_last_name_cols;
	int student_id_cols;
	size_t student_list_size;

	bool done_calculations;
	bool course_selected;
};

class StateRegisterList : public State
{
public:
	StateRegisterList();
	~StateRegisterList();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();

private:
	size_t course_current_index;
	int course_current_list_page;
	int course_total_list_pages;
	int course_name_cols;
	int course_plan_cols;
	int course_id_cols;
	size_t course_list_size;

	size_t page_size;

	size_t student_current_index;
	int student_current_list_page;
	int student_total_list_pages;
	int student_first_name_cols;
	int student_last_name_cols;
	int student_id_cols;
	size_t student_list_size;

	bool done_calculations;
	bool course_selected;
};

class StateDefaultWorkspace : public State
{
public:
	StateDefaultWorkspace();
	~StateDefaultWorkspace();
	void mainWindowHandler();
	void inputHandler(int input);
	void formatHandler();
	State_list_t destination_state;
	bool load_success;

private:
	bool main_window_is_printed;
};

class StateSaveToFile : public State, public FileIO
{
public:
	StateSaveToFile();
	~StateSaveToFile();
	state_return_t stateFunction(int input);

private:
	Error_code_t printToCourse();
	Error_code_t printToStudent();
	std::fstream output_student_f;
	std::fstream output_course_f;
};

class StateLoadFromFile : public State, public FileIO
{
public:
	StateLoadFromFile();
	~StateLoadFromFile();
	state_return_t stateFunction(int input);

private:
	Error_code_t loadStudent();
	Error_code_t loadCourse();
	std::fstream input_student_f;
	std::fstream input_course_f;
};

class StateInitialCheck : public State, public FileIO
{
public:
	StateInitialCheck();
	~StateInitialCheck();
	state_return_t stateFunction(int input);

private:
};

class StateExit : public State
{
public:
	StateExit();
	~StateExit();
	state_return_t stateFunction(int input);

private:
};

extern State *states_table[NUMBER_OF_STATES];

extern StateMainMenu Main_Menu;
extern StateStudentsMenu Students_Menu;
extern StateCoursesMenu Courses_Menu;
extern StateRegisterationMenu Registeration_Menu;
extern StateReportsMenu Reports_Menu;
extern StateDefaultWorkspace Default_Workspace;
extern StateAddStudent Add_Student;
extern StateStudentsList Students_List;
extern StateAddTheoreticalCourse Add_Theoretical_Course;
extern StateAddPracticalCourse Add_Practical_Course;
extern StateCoursesList Courses_List;
extern StateRemoveStudent Remove_Student;
extern StateNotificationStudentAddSuccess Notification_Student_Add_Success;
extern StateNotificationStudentAddFail Notification_Student_Add_Fail;
extern StateNotificationCourseAddSuccess Notification_Course_Add_Success;
extern StateNotificationPracticalCourseAddFail Notification_Practical_Course_Add_Fail;
extern StateNotificationTheoreticalCourseAddFail Notification_Theoretical_Course_Add_Fail;
extern StateNotificationRemoveStudentPopup Notification_Remove_Student_Popup;
extern StateNotificationRemoveCoursePopup Notification_Remove_Course_Popup;
extern StateRemoveCourse Remove_Course;
extern StateSearchStudent Search_Student;
extern StateSearchCourse Search_Course;
extern StateCourseRegister Course_Register;
extern StateNotificationRegisterSuccess Notification_Register_Success;
extern StateNotificationRegisterFail Notification_Register_Fail;
extern StateCourseUnregister Course_Unregister;
extern StateNotificationUnregisterSuccess Notification_Unregister_Success;
extern StateGradeEntry Grade_Entry;
extern StateNotificationGradeEntrySuccess Notification_Grade_Entry_Success;
extern StateNotificationGradeEntryFail Notification_Grade_Entry_Fail;
extern StateCourseGrades Course_Grades;
extern StateRegisterList Register_List;
extern StateSaveToFile Save_To_File;
extern StateLoadFromFile Load_From_File;
extern StateNotificationStudentAddDuplicate Notification_Student_Add_Duplicate;
extern StateNotificationTheoreticalCourseAddDuplicate Notification_Theoretical_Course_Add_Duplicate;
extern StateNotificationPracticalCourseAddDuplicate Notification_Practical_Course_Add_Duplicate;
extern StateInitialCheck Initial_Check;
extern StateExit Exit;

#endif