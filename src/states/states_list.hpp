#ifndef __STATE_LIST_H__
#define __STATE_LIST_H__

enum class State_list_t
{
	State_Main_Menu,
	State_Students_Menu,
	State_Courses_Menu,
	State_Registeration_Menu,
	State_Reports_Menu,
	State_Default_Workspace,
	State_Add_Student,
	State_Students_List,
	State_Add_Theoretical_Course,
	State_Add_Practical_Course,
	State_Courses_List,
	State_Remove_Student,
	State_Notification_Student_Add_Success,
	State_Notification_Student_Add_Fail,
	State_Notification_Course_Add_Success,
	State_Notification_Practical_Course_Add_Fail,
	State_Notification_Theoretical_Course_Add_Fail,
	State_Notification_Remove_Student_Popup,
	State_Notification_Remove_Course_Popup,
	State_Remove_Course,
	State_Search_Student,
	State_Search_Course,
	State_Course_Register,
	State_Notification_Register_Success,
	State_Notification_Register_Fail,
	State_Course_Unregister,
	State_Notification_Unregister_Success,
	State_Grade_Entry,
	State_Notification_Grade_Entry_Success,
	State_Notification_Grade_Entry_Fail,
	State_Course_Grades,
	State_Register_List,
	State_Save_To_File,
	State_Load_From_File,
	State_Notification_Student_Add_Duplicate,
	State_Notification_Theoretical_Course_Add_Duplicate,
	State_Notification_Practical_Course_Add_Duplicate,
	State_Initial_Check,
	State_Exit
};

#endif