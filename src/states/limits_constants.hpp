#ifndef __LIMITS_CONSTANTS__
#define __LIMITS_CONSTANTS__

inline constexpr int BLACK_ON_WHITE = 1;
inline constexpr int WHITE_ON_BLUE = 2;
inline constexpr int WHITE_ON_GREEN = 3;
inline constexpr int WHITE_ON_BLACK = 4;
inline constexpr int BLUE_ON_YELLOW = 5;
inline constexpr int WHITE_ON_RED = 6;
inline constexpr int RED_ON_WHITE = 7;
inline constexpr int YELLOW_ON_BLACK = 8;
inline constexpr int GREEN_ON_BLACK = 9;
inline constexpr int GREEN_ON_WHITE = 10;
inline constexpr int RED_ON_BLACK = 11;
inline constexpr int BLACK_ON_RED = 12;

inline constexpr int MENU_COLOR = WHITE_ON_BLACK;
inline constexpr int MENU_HIGHLIGHT_COLOR = GREEN_ON_BLACK;
inline constexpr int MENU_TITLE_BAR_COLOR_ON = GREEN_ON_BLACK;
inline constexpr int MENU_TITLE_BAR_COLOR_OFF = WHITE_ON_BLACK;
inline constexpr int MENU_FOOTER_BAR_COLOR = WHITE_ON_GREEN;
inline constexpr int WORKSPACE_COLOR = BLACK_ON_WHITE;
inline constexpr int WORKSPACE_TITLE_BAR_COLOR = BLACK_ON_WHITE;
inline constexpr int WORKSPACE_TITLE_BAR_COLOR_ON = GREEN_ON_BLACK;
inline constexpr int WORKSPACE_TITLE_BAR_COLOR_OFF = WHITE_ON_BLACK;
inline constexpr int WORKSPACE_FOOTER_BAR_COLOR = WHITE_ON_GREEN;
inline constexpr int WORKSPACE_HIGHLIGHT_COLOR = WHITE_ON_GREEN;
inline constexpr int WORKSPACE_BUTTON_COLOR_OFF = WHITE_ON_BLACK;
inline constexpr int WORKSPACE_BUTTON_COLOR_ON = WHITE_ON_GREEN;
inline constexpr int WORKSPACE_X_COLOR = RED_ON_WHITE;
inline constexpr int WORKSPACE_REPORT_LIST_HIGHLIGHT = WHITE_ON_GREEN;
inline constexpr int WORKSPACE_REMOVE_LIST_HIGHLIGHT = WHITE_ON_RED;
inline constexpr int WORKSPACE_LIST_GUIDE = WHITE_ON_BLACK;
inline constexpr int NOTIFICATION_COLOR = WHITE_ON_BLACK;
inline constexpr int NOTIFICATION_TITLE_BAR_COLOR = BLACK_ON_WHITE;
inline constexpr int NOTIFICATION_TITLE_BAR_COLOR_ON = GREEN_ON_BLACK;
inline constexpr int NOTIFICATION_FAIL_TITLE_BAR_COLOR_ON = RED_ON_BLACK;
inline constexpr int NOTIFICATION_TITLE_BAR_COLOR_OFF = BLACK_ON_WHITE;
inline constexpr int NOTIFICATION_FOOTER_BAR_COLOR = WHITE_ON_GREEN;
inline constexpr int NOTIFICATION_FAIL_FOOTER_BAR_COLOR = WHITE_ON_RED;
inline constexpr int NOTIFICATION_BUTTON_COLOR_OFF = BLACK_ON_WHITE;
inline constexpr int NOTIFICATION_BUTTON_COLOR_ON = WHITE_ON_GREEN;
inline constexpr int NOTIFICATION_FAIL_BUTTON_COLOR_ON = BLACK_ON_RED;

inline constexpr int MAXIMUM_FIRST_NAME_LENGTH = 50;
inline constexpr int MAXIMUM_LAST_NAME_LENGTH = 50;
inline constexpr int MAXIMUM_STUDENT_ID_LENGTH = 25;
inline constexpr int MAXIMUM_COURSE_NAME_LENGTH = 50;
inline constexpr int MAXIMUM_COURSE_ID_LENGTH = 25;
inline constexpr int MAXIMUM_PLAN_LENGTH = 250;
inline constexpr int MAXIMUM_COURSE_GRADE_STRING_LENGTH = 4;

inline constexpr int MAXIMUM_GRADE = 20;
inline constexpr int MINIMUM_GRADE = 0;

inline constexpr int NOTIFICATION_ROWS = 8;
inline constexpr int NOTIFICATION_COLS = 30;

inline constexpr int MENU_COLS = 25;

inline constexpr int TITLE_BAR_WIDTH = 3;
inline constexpr int FOOTER_BAR_WIDTH = 1;

inline constexpr int LIST_GUIDE_LINE_FROM_TOP = 1;
inline constexpr int SEARCH_LIST_GUIDE_LINE_FROM_TOP = 5;
inline constexpr int LIST_BEGIN_LINE_FROM_TOP = LIST_GUIDE_LINE_FROM_TOP + 1;
inline constexpr int LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM = 1;

inline constexpr int REGISTER_LIST_GUIDE_LINE_FROM_TOP = 3;
inline constexpr int REGISTER_LIST_BEGIN_LINE_FROM_TOP = REGISTER_LIST_GUIDE_LINE_FROM_TOP + 1;
inline constexpr int REGISTER_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM = 1;

inline constexpr int GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP = 3;
inline constexpr int GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP = GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP + 1;
inline constexpr int GRADES_REPORT_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM = 1;

inline constexpr int LAST_NAME_COLS_PERCENT = 40;
inline constexpr int FIRST_NAME_COLS_PERCENT = 40;
inline constexpr int ID_COLS_PERCENT = 20;

inline constexpr int COURSE_NAME_COLS_PERCENT = 30;
inline constexpr int COURSE_PLAN_COLS_PERCENT = 50;
inline constexpr int COURSE_ID_COLS_PERCENT = 20;

inline constexpr int GRADES_MINIMUM = 0;
inline constexpr int GRADES_MAXIMUM = 20;
inline constexpr int GRADES_NOT_ENTERED = -1;

#endif