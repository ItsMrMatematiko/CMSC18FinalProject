// Hi Guys! Mao ni atong gamiton na file for the final project. Naka live share na ni from VSCode.
// What will happen is diri ta mag code sa VSCode, pero ang pag run sa output is imatch nato sa DevC++
// since naay uban function nga dili magamit diri sa C++ pero magamit sa Dev. Mao lang ehehe~


// Checker for nicknames (Strictly No Duplicates)
// Fix Format (not urgent)
// Checker for Existing Schedules (Adding Schedules Function)
// File Handling (List of Members, List of Sunday Schedules, List of Weekdays Schedules)
// Input Error Handling (Not finishing fill-ups and skipping data)
// (Separate Feature of the App) Mark Server Attendance During Serve


// scanf("%[^\n]s",str); *PARA NI SA STRING INPUTS NGA IAPIL ANG SPACES


// Nicknames should be unique identifiers 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

#define MAX 50

// STRUCTURE DECLARATIONS
typedef struct HolyMass{
    int Year, Month, Date, MassNumber, isScheduled[4];
    char FirstReader[16], SecondReader[16], POF[16], Commentator[16];
} HolyMass;

typedef struct MinistryMember{
    int Index, YearMembership, MonthMembership, DayMembership;
    char Surname[21], FirstName[31], MiddleName[16], Nickname[16], Sex;
} MinistryMember;



// VARIABLE DECLARATIONS
int savedSchedulesCount, idxCtr01, idxCtr02;
HolyMass *SundaySchedules = NULL, *WeekdaySchedules = NULL;
MinistryMember *MinistryMembers = NULL;
FILE *MinistryMembersDatabase, *SundayScheduleDatabase, *WeekdayScheduleDatabase;
int genderoption;
int choice;
int option;
int ministryMemberCount = 0;
int sundayScheduleCount = 0;
int weekdayScheduleCount = 0;
char temp, toContinue;



// FUNCTION PROTOTYPE DECLARATIONS
void PrintAppHeader();
void ClearInputBuffer();
void ExecuteProgram();
char MainMenu();
void AccessMinistryMembers();
void DeleteMinistryMember();
void AccessWeekdaysSchedules();
void AccessSundaysSchedules();
void AddSundaySchedule();
void ViewSundaySchedule();
int CheckForDaysInAMonth(int, int);
int isSunday(int, int, int);
void PrintCalendar(int, int);
void ExitProgram(int);
void AddRecord();
void AssignMonth(int, char*);
void CheckNewRecordEqualsSignChar(char*);
void CheckNewRecordEqualsSignChar2(char);
void CheckNewRecordEqualsSignInt(int);
void PrintMembersWithNumberAndNickname();
void DisplayAll();
void BubbleSort();
void BubbleSortGender(int genderoption);
void YearSort();
void GenderSort();
void ImportMinistryMembers();
void ImportSundaySchedules();
void CheckSundayServerInputZero(int*);
void ExportMinistryMembers();
void ExportSundaySchedules();
int compareIndices(const void *a, const void *b);


// THE PROGRAM
int main(){
    MinistryMembers = (MinistryMember *)realloc(MinistryMembers, MAX * sizeof(MinistryMember));
	ImportMinistryMembers(); 
	ImportSundaySchedules();      
    ExecuteProgram();
}

void PrintAppHeader(){
    system("cls");
    printf("===============================================================================================\n");
    printf("OUR LADY OF FATIMA PARISH\n");
    printf("Fatima St., Brgy. 24-C, Davao City, Philippines, 8000\n");
    printf("Lectors' and Commentators' Ministry\n");
    printf("DIGITAL SERVICE SCHEDULING APPLICATION\n");
    printf("===============================================================================================\n");
}

void ExecuteProgram(){	
    if (ministryMemberCount > 0) BubbleSort();    
    PrintAppHeader();

    char choiceOfAction = MainMenu();

    switch(choiceOfAction){
        case '1': AccessMinistryMembers(); break;
        case '2': AccessWeekdaysSchedules(); break;            
        case '3': AccessSundaysSchedules(); break;            
        case '=': ExitProgram(0); break;
    }

    printf("\tDo you wish to perform other actions in this program?.\n");
	printf("\t\t[Y] or [y] for YES\n"); 
	printf("\t\tAny character for NO\n"); 
	printf("\n\t\tUser Input: ");
	scanf(" %c", &toContinue);
	PrintAppHeader();
	ClearInputBuffer();	
	printf("==========================================================================================================\n");
	
	if(toContinue == 'Y' || toContinue == 'y'){ system("cls"); ExecuteProgram(); }
	else ExitProgram(0);
}

char MainMenu(){    
    char selectedOption;

    printf("\n\tPROGRAM OPTIONS\n\n");
    printf("\t\t[1] Access Ministry Members\n");
    printf("\t\t[2] Access Weekdays Scheduled Servers\n");
    printf("\t\t[3] Access Sundays Scheduled Servers\n");
    printf("\t\t[=] Exit the Program\n\n");
        
    do{
        printf("\tWhich of the following actions would you like to do? ");
        scanf(" %c", &selectedOption);
        ClearInputBuffer();          

        if(selectedOption != '1' && selectedOption != '2' && selectedOption != '3' && selectedOption != '='){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }        
    }while(selectedOption != '1' && selectedOption != '2' && selectedOption != '3' && selectedOption != '=');   
        
    return selectedOption;
}

// Record Ministry Members
void AccessMinistryMembers(){
    PrintAppHeader();
    do {
        printf("\n\tPROGRAM OPTIONS\n\n");
        printf("\t\t[1] Add New Records\n");
        printf("\t\t[2] View All Existing Records\n");
        printf("\t\t[3] Delete a Member\n");
        printf("\t\t[=] Go Back to Main Menu\n\n");
        
        printf("\tEnter Choice here: ");
        scanf(" %c", &choice);
        ClearInputBuffer();  
            
        if(choice != '1' && choice != '2' && choice != '3' && choice != '='){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }   
    } while (choice != '1' && choice != '2' && choice != '3' && choice != '=');

    switch(choice){
        case '1':
            AddRecord();
            BubbleSort();
            break;

        case '2':
            DisplayAll();
            break;

        case '3':
            DeleteMinistryMember();
            break;

        case '=':
            ExecuteProgram();
            ExitProgram(0);
            break;

        default:
            break;
    } 
}

void AddRecord(){
	char tempSurname[21], tempFirstName[31], tempMiddleName[16], tempNickname[16], tempSex;
	int tempYear, tempMonth, tempDay, nicknameExists = 0, daysInMonth;
	
    if (ministryMemberCount < MAX)
    {
    	PrintAppHeader();
        printf("\n\t-----------------------------------------------------------------");
        printf("\n\n");
        printf("\tIf you wish to return to the Program options please enter [0]\n\n");
            
        printf("\tEnter Member's Surname: ");
        scanf(" %[^\n]s", &tempSurname);
        CheckNewRecordEqualsSignChar(tempSurname);
        ClearInputBuffer();  
		
        printf("\tEnter Member's First Name: ");
        scanf(" %[^\n]s", &tempFirstName);
        CheckNewRecordEqualsSignChar(tempFirstName);
        ClearInputBuffer();  
		    
        printf("\tEnter Member's Middle Name: ");
        scanf(" %[^\n]s", &tempMiddleName);
        CheckNewRecordEqualsSignChar(tempMiddleName);
        ClearInputBuffer();  
        
        do {
        	nicknameExists = 0;
        	
            printf("\tEnter Member's Nickname: ");
            scanf(" %[^\n]s", &tempNickname);
            CheckNewRecordEqualsSignChar(tempNickname);
            ClearInputBuffer();

            for (idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01++) {
                if (strcasecmp(tempNickname, MinistryMembers[idxCtr01].Nickname) == 0) {
                    nicknameExists = 1;
                    printf("\n\tNickname already exists. Please enter a different nickname.\n");
                }
            }

            if (nicknameExists == 0) {
                break;
            }
        } while (nicknameExists = 1);
            
        do {    
            printf("\tSex (M or F): ");
            scanf(" %c", &tempSex);
            
            if (toupper(tempSex) != 'M' && toupper(tempSex) != 'F') {
                printf("\t\tInvalid input, please choose between M or F.\n");
            }

            tempSex = toupper(tempSex);

            CheckNewRecordEqualsSignChar2(tempSex);
            ClearInputBuffer();
    	} while (tempSex != 'M' && tempSex != 'F');  
        
		do {    
            printf("\tEnter year of Membership: ");
            scanf("%d", &tempYear);
            CheckNewRecordEqualsSignInt(tempYear);
                
            if (tempYear < 1952) 
                printf("\t\tInvalid year. The church has not yet existed at this time.\n");

            else if (tempYear > 2024)
                printf("\t\tInvalid year. Please enter a year between 1952 and 2024.\n");

            else 
                break;
        } while (tempYear < 1952 || tempYear > 2024);
            
        do {
            printf("\tEnter Month of Membership (1-12): ");
            scanf("%d", &tempMonth);
            CheckNewRecordEqualsSignInt(tempMonth);
            ClearInputBuffer();  
            if (1 > tempMonth || 12 < tempMonth)
                printf("\t\tInvalid Month please choose between 1-12\n");

            else
                break;

        } while (1 > tempMonth || 12 < tempMonth);
            
        do {
    		printf("\tEnter day of Membership: ");
    		scanf("%d", &tempDay);
    		CheckNewRecordEqualsSignInt(tempDay);
    		ClearInputBuffer();

    		daysInMonth = CheckForDaysInAMonth(tempMonth, tempYear);

    		if (tempDay < 1 || tempDay > daysInMonth)
        		printf("\t\tInvalid day for the selected month. Please try again.\n");
    		
            else
        	    break;
        } while (tempDay < 1 || tempDay > daysInMonth);

		MinistryMembers[ministryMemberCount].Index = ministryMemberCount;
		strcpy(MinistryMembers[ministryMemberCount].Surname, tempSurname);
		strcpy(MinistryMembers[ministryMemberCount].FirstName, tempFirstName);
		strcpy(MinistryMembers[ministryMemberCount].MiddleName, tempMiddleName);
		strcpy(MinistryMembers[ministryMemberCount].Nickname, tempNickname);
		MinistryMembers[ministryMemberCount].Sex = tempSex;
		MinistryMembers[ministryMemberCount].YearMembership = tempYear;
		MinistryMembers[ministryMemberCount].MonthMembership = tempMonth;
		MinistryMembers[ministryMemberCount].DayMembership = tempDay;
        ministryMemberCount++;
            
        }
        else    
            printf("\t\tAction declined. Maximum amount of records acquired.");
    
    
    printf("\n");
    printf("\t-----------------------------------------------------------------\n\n");

}

void DeleteMinistryMember(){
    int DeletedMember;
    PrintAppHeader();
    DisplayAll();

    printf("\n\tEnter the number of the member you want to delete (0 to cancel): ");
    scanf("%d", &DeletedMember);
    ClearInputBuffer();

    if (DeletedMember >= 1 && DeletedMember <= ministryMemberCount) {
        for (idxCtr01 = DeletedMember - 1; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
            MinistryMembers[idxCtr01] = MinistryMembers[idxCtr01 + 1];
            MinistryMembers[idxCtr01].Index = idxCtr01 + 1; 
        }

        ministryMemberCount--;
        printf("\n\tMember deleted successfully.\n");
    } else if (DeletedMember != 0) {
        printf("\n\tInvalid member number. No changes made.\n");
    }

    printf("\n\t-----------------------------------------------------------------\n\n");
}

void CheckNewRecordEqualsSignChar(char* temp){
	 if (strcmp(temp, "0") == 0) {
                AccessMinistryMembers();
                ExitProgram(0);
}
}

void CheckNewRecordEqualsSignInt(int temp){
	 if (temp == 0) 
	 {
                AccessMinistryMembers();
                ExitProgram(0);
}
}

void CheckNewRecordEqualsSignChar2(char temp){
    if (temp == '0') {
        AccessMinistryMembers();
        ExitProgram(0);
    }
}

void BubbleSort() {
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            if (strcmp(strupr(MinistryMembers[idxCtr01].Surname), strupr(MinistryMembers[idxCtr02].Surname)) > 0) {
                //swap
                MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}

void YearSort(){
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            if (MinistryMembers[idxCtr01].YearMembership < MinistryMembers[idxCtr02].YearMembership) {
                //swap
                MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}

void GenderSort(){
	PrintAppHeader();
	do{
    printf("\n\tSort by Gender:\n");
    printf("\t\t[1] Male first\n");
    printf("\t\t[2] Female first\n");
    printf("\t\t[0] Return to previous section\n");

    printf("\tEnter your choice: ");
    scanf("%d", &genderoption);
    ClearInputBuffer(); 
    
    if (genderoption == 0){
		DisplayAll();
	}
	
    if (genderoption != 2 && genderoption != 1 && genderoption != 0){
    	printf("\t\t\t Invalid input. Please try again");
	}
    }while (genderoption != 2 && genderoption != 1 && genderoption != 0);
     
        
    BubbleSortGender(genderoption);
}

void BubbleSortGender(int genderoption) {
	
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            if ((genderoption == 1 && MinistryMembers[idxCtr01].Sex == 'F' && MinistryMembers[idxCtr02].Sex != 'F') ||
                (genderoption == 2 && MinistryMembers[idxCtr01].Sex == 'M' && MinistryMembers[idxCtr02].Sex == 'F')) {
                // Swap
                struct MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}

void DisplayAll()	{        
    char monthName[10];
        
    PrintAppHeader();
    
    do {
        printf("\n\tHow do you wish to view the record?\n\n");
        printf("\t\t[1] Default (Ascending Alphabetical Order)\n");
        printf("\t\t[2] Year Sort (Recent Members)\n");
        printf("\t\t[3] Gender Sort\n");
        printf("\t\t[0] Return to Program Options\n\n"); 
                   
        printf("\tInput Choice: ");
        scanf("%d", &option);
        ClearInputBuffer();  
                
        if (0 > option || option > 3) 
            printf("\t\tInvalid input. Please try again\n");                    
        else 
            break;
    } while (0 > option || option > 3);
                
    switch (option){
        case 1: BubbleSort(); break;
        case 2: YearSort(); break;
        case 3: GenderSort(); break;
        case 0: AccessMinistryMembers(); ExitProgram(0);
        default: printf("\tInvalid option\n");
    }
            
    PrintAppHeader();
    printf("\t|=========================================================================|\n");
    printf("\t|                             MEMBER DATABASE                             |\n");
    printf("\t|-------------------------------------------------------------------------|\n");
        
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01++) {
        AssignMonth(MinistryMembers[idxCtr01].MonthMembership, monthName);            
        printf("\t| %2d |  Surname: %-35s   | Sex: %-9c   |\n", idxCtr01 + 1, MinistryMembers[idxCtr01].Surname, MinistryMembers[idxCtr01].Sex);
        printf("\t|    |  First Name: %-32s   | Year: %04d       |\n", MinistryMembers[idxCtr01].FirstName, MinistryMembers[idxCtr01].YearMembership);
        printf("\t|    |  Middle Name: %-32s  | Month: %-9s |\n", MinistryMembers[idxCtr01].MiddleName, monthName);
        printf("\t|    |  Nickname: %-33s    | Day: %02d          |\n", MinistryMembers[idxCtr01].Nickname, MinistryMembers[idxCtr01].DayMembership);   
        printf("\t|-------------------------------------------------------------------------|\n");
    }

    printf("\t====================================================================================\n\n");  
}

void PrintMembersWithNumberAndNickname() {
    printf("\t\n\tMembers with Number and Nickname\n");

    printf("\t| No. | Surname            | Nickname         |\t\t| No. | Surname            | Nickname         |\n");
    printf("\t|-----|--------------------|------------------|\t\t|-----|--------------------|------------------|\n");

    // Print two tables side by side
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01 += 2) {
        printf("\t| %-3d | %-18s | %-16s |\t\t", idxCtr01 + 1, MinistryMembers[idxCtr01].Surname, MinistryMembers[idxCtr01].Nickname);

        // Check if there is a second member in the current row
        if (idxCtr01 + 1 < ministryMemberCount) {
            printf("| %-3d | %-18s | %-16s |\n", idxCtr01 + 2, MinistryMembers[idxCtr01 + 1].Surname, MinistryMembers[idxCtr01 + 1].Nickname);
        } else {
            printf("|\n");
        }
        printf("\t|---------------------------------------------|\t\t|---------------------------------------------|\n");
    }

}

int compareIndices(const void *a, const void *b) {
    const MinistryMember *ma = (const MinistryMember *)a;
    const MinistryMember *mb = (const MinistryMember *)b;

    if (ma->Index < mb->Index) return -1;
	else if (ma->Index > mb->Index) return 1; 
	else return 0;
}

// Shared Scheduling Functions
int GetSelectedMonth(){
    int input;

    // Preliminary code blocks to access real-time.
    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;

    printf("\tMonth Options\n");
    printf("\t[1] January\t\t [5] May \t\t [9] September\n");
    printf("\t[2] February\t\t [6] June \t\t [10] October\n");
    printf("\t[3] March\t\t [7] July \t\t [11] November\n");
    printf("\t[4] April\t\t [8] August \t\t [12] December\n");
    printf("\t[0] Go Back to Previous Menu\n\n");
    
    do{
        printf("\tEnter the Month to be placed with a Schedule (Only for Year %d) [1 - 12]: ", currentYear);
        scanf(" %d", &input);
        ClearInputBuffer();
        
        if(input < 0 || input > 12) printf("\t\t\tYou provided an invalid input. Please try again.\n\n");
        else return input;
    }while(input < 0 || input > 12);
}

int GetSelectedDate(int month){
    int input;

    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;
    int currentDate = current_time->tm_mday;
    int currentMonth = current_time->tm_mon + 1;

    do{
        printf("\tWhich day of the Month will we schedule service? ");            
        scanf(" %d", &input);
        ClearInputBuffer();

        if(input == 0)  return 0;

        else if(input < currentDate && (month <= currentMonth && input < currentDate)){
            printf("\t\tYou entered a past date. Please try again.\n\n");
        }
        
        else if(input < 0 || input > CheckForDaysInAMonth(month, currentYear)){
            printf("\t\tYou entered an invalid date. Please try again.\n\n");
        }

        else return input;
    }while(input < 0 || input > CheckForDaysInAMonth(month, currentYear) || (input < currentDate && (month <= currentMonth && input < currentDate)));
   
}

void AssignMonth(int monthCode, char *monthName){	
	switch (monthCode) {
	    case 1: strcpy(monthName, "January"); break;
	    case 2: strcpy(monthName, "February"); break;
	    case 3: strcpy(monthName, "March"); break;
	    case 4: strcpy(monthName, "April"); break;
	    case 5: strcpy(monthName, "May"); break;
	    case 6: strcpy(monthName, "June"); break;
	    case 7: strcpy(monthName, "July"); break;
	    case 8: strcpy(monthName, "August"); break;
	    case 9: strcpy(monthName, "September"); break;
	    case 10: strcpy(monthName, "October"); break;
	    case 11: strcpy(monthName, "November"); break;
	    case 12: strcpy(monthName, "December"); break;
    }
}

void PrintMonth(int monthCode){	
	switch(monthCode){
        case 1: printf("January"); break;
        case 2: printf("February"); break;
        case 3: printf("March"); break;
        case 4: printf("April"); break;
        case 5: printf("May"); break;
        case 6: printf("June"); break;
        case 7: printf("July"); break;
        case 8: printf("August"); break;
        case 9: printf("September"); break;
        case 10: printf("October"); break;
        case 11: printf("November"); break;
        case 12: printf("December"); break;
    }
}

int AskServerIndex(int *indexInput){
    scanf(" %d", indexInput);
    ClearInputBuffer();
    CheckSundayServerInputZero(indexInput);

    if(*indexInput > ministryMemberCount){
        printf("\t\tYou provided an input beyond the number of members. Please try again.\n\n");
    }

    else return *indexInput;
}


// Sunday Scheduling Functions
void AccessSundaysSchedules(){
    char selectedOption;

    PrintAppHeader();
    printf("\n\tSUNDAY SCHEDULES\n\n");
    printf("\t\t[1] Add/Modify Server Schedules for a Specific Sunday\n");
    printf("\t\t[2] View Scheduled Servers for a Specific Sunday\n");
    printf("\t\t[=] Go Back\n");
    
    do{
        printf("\n\tWhich of the following actions would you like to do? ");
        scanf(" %c", &selectedOption);
        ClearInputBuffer();

        if(selectedOption != '1' && selectedOption != '2' && selectedOption != '='){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }   
        
    }while(selectedOption != '1' && selectedOption != '2' && selectedOption != '=');

    switch(selectedOption){
        case '1':
            AddSundaySchedule();
            break;

        case '2':
            ViewSundaySchedule();
            break;

        case '=':
            ExecuteProgram();
            break;

        default:
            break;
    }  
}

void PrintSundayTimeSlot(int timeSlotCode){
    switch(timeSlotCode){
        case 1: printf("05:30 AM"); break;
        case 2: printf("07:00 AM"); break;
        case 3: printf("08:30 AM"); break;
        case 4: printf("03:00 PM"); break;
        case 5: printf("04:30 PM"); break;
        case 6: printf("06:00 PM"); break;
    }
}

void CheckSundayServerInputZero(int *serverIndex){
    if(*serverIndex == 0){
        system("cls");
        AccessSundaysSchedules();
        ExitProgram(0);
    }
}

int AssignServer(int year, int month, int date, int timeSlotIndex, int sundayScheduleIndex, int ministryMemberIndex, int servicePosition){    
	if(ministryMemberIndex > 0){
        switch(servicePosition){
            case 0:                
				strcpy(SundaySchedules[sundayScheduleIndex].FirstReader, MinistryMembers[ministryMemberIndex - 1].Nickname);
                SundaySchedules[sundayScheduleIndex].isScheduled[0] = 1;                
                break;
            case 1:
				strcpy(SundaySchedules[sundayScheduleIndex].SecondReader, MinistryMembers[ministryMemberIndex - 1].Nickname);
                SundaySchedules[sundayScheduleIndex].isScheduled[1] = 1; 
                break;
            case 2:
                strcpy(SundaySchedules[sundayScheduleIndex].POF, MinistryMembers[ministryMemberIndex - 1].Nickname);
                SundaySchedules[sundayScheduleIndex].isScheduled[2] = 1; 
                break;
            case 3:
                strcpy(SundaySchedules[sundayScheduleIndex].Commentator, MinistryMembers[ministryMemberIndex - 1].Nickname);
                SundaySchedules[sundayScheduleIndex].isScheduled[3] = 1; 
                break;
        }
    }    

    else{
        switch(servicePosition){
            case 0: 
                strcpy(SundaySchedules[sundayScheduleIndex].FirstReader, "NONE"); 
                SundaySchedules[sundayScheduleIndex].isScheduled[0] = 0; 
                break;
            case 1: 
                strcpy(SundaySchedules[sundayScheduleIndex].SecondReader, "NONE"); 
                SundaySchedules[sundayScheduleIndex].isScheduled[1] = 0; 
                break;
            case 2: 
                strcpy(SundaySchedules[sundayScheduleIndex].POF, "NONE"); 
                SundaySchedules[sundayScheduleIndex].isScheduled[2] = 0; 
                break;
            case 3: 
                strcpy(SundaySchedules[sundayScheduleIndex].Commentator, "NONE"); 
                SundaySchedules[sundayScheduleIndex].isScheduled[3] = 0; 
                break;
        }        
    }
    
    SundaySchedules[sundayScheduleIndex].Year = year;
    SundaySchedules[sundayScheduleIndex].Month = month;
    SundaySchedules[sundayScheduleIndex].Date = date;
    SundaySchedules[sundayScheduleIndex].MassNumber = timeSlotIndex;
}

void InputServers(int *serverOne, int *serverTwo, int *serverThree, int *serverFour){
    // Designate Servers
        do{
            printf("\t First Reader: ");
            AskServerIndex(serverOne);
        }while(*serverOne > 50);    

        do{
            printf("\t Second Reader: ");
            AskServerIndex(serverTwo);

            if(*serverOne == *serverTwo && *serverTwo > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverTwo > 50 || (*serverOne == *serverTwo && *serverTwo > 0));
        
        do{
            printf("\t Prayers of the Faithful (POF): ");
            AskServerIndex(serverThree);

            if((*serverOne == *serverThree || *serverTwo == *serverThree) && *serverThree > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverThree > 50 || ((*serverOne == *serverThree || *serverTwo == *serverThree) && *serverThree > 0));

        do{
            printf("\t Commentator: ");
            AskServerIndex(serverFour);

            if((*serverOne == *serverFour || *serverTwo == *serverFour || *serverThree == *serverFour) && *serverFour > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverThree > 50 || (*serverOne == *serverFour || *serverTwo == *serverFour || *serverThree == *serverFour) && *serverFour > 0);
}

void AddSundaySchedule(){
    int inputMonth, inputSunday, inputTimeSlot, toAssign = 0, dateHasSchedule, scheduleEntryMatch, areSlotsScheduled = 0;
    int serverOne, serverTwo, serverThree, serverFour;
    
    // Preliminary code blocks to access real-time.
    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;
    
    PrintAppHeader();
    printf("\tSUNDAY SERVICE SCHEDULING\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Check for selected month
    inputMonth = GetSelectedMonth();
    
    if(inputMonth == 0){
        system("cls");
        AccessSundaysSchedules();
        ExitProgram(0);
    }
    
    // Print the calendar for the selected month on year 2024
    PrintCalendar(inputMonth, currentYear);

    // Get the Sunday to be placed with a schedule
    do{
        inputSunday = GetSelectedDate(inputMonth);

        if(inputSunday == 0){
            system("cls");
            AccessSundaysSchedules();
            ExitProgram(0);
        }

        else if(!isSunday(currentYear, inputMonth, inputSunday)){
            printf("\t\tYou entered a date that is not a Sunday. Please try again.\n\n");
        }
    }while(!isSunday(currentYear, inputMonth, inputSunday));

    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    printf("\tPlease select a time slot for the Service Scheduling\n");
    printf("\t\t[1] 05:30 AM \t\t[4] 03:00 PM\n");
    printf("\t\t[2] 07:00 AM \t\t[5] 04:30 PM\n");
    printf("\t\t[3] 08:30 AM \t\t[6] 06:00 PM\n");
    printf("\t\t[0] Go back to Access Sunday Schedules Menu\n\n");

    // Get the specified time slot for scheduling
    do{
        printf("\tSelected Time Slot: ");
        scanf(" %d", &inputTimeSlot);
        ClearInputBuffer();

        if(inputTimeSlot < 0 || inputTimeSlot > 6){
            printf("\t\t\tYou provided an invalid input. Please try again.\n");
        }		

        else if(inputTimeSlot == 0){
            system("cls");
            AccessSundaysSchedules();
            ExitProgram(0);
        }
    }while(inputTimeSlot < 0 || inputTimeSlot > 6);
    
    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    // Shows the list of all nicknames
    PrintMembersWithNumberAndNickname();

    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    // Check if schedule already exists. If it does, then print the available schedule and then let user select schedule to modify
    dateHasSchedule = 0;
    scheduleEntryMatch = 0;

    for(idxCtr01 = 0; idxCtr01 < sundayScheduleCount; idxCtr01++){
        scheduleEntryMatch = SundaySchedules[idxCtr01].Year == currentYear &&
                             SundaySchedules[idxCtr01].Month == inputMonth &&
                             SundaySchedules[idxCtr01].Date == inputSunday &&
                             SundaySchedules[idxCtr01].MassNumber == inputTimeSlot;
        
        dateHasSchedule = SundaySchedules[idxCtr01].isScheduled[0] == 1 ||
                          SundaySchedules[idxCtr01].isScheduled[1] == 1 ||
                          SundaySchedules[idxCtr01].isScheduled[2] == 1 ||
                          SundaySchedules[idxCtr01].isScheduled[3] == 1;

        if(scheduleEntryMatch && dateHasSchedule){
            printf("\n\n\tCURRENT SERVERS\n");
            printf("\t\tFirst Reader: %s\n", SundaySchedules[idxCtr01].FirstReader);
            printf("\t\tSecond Reader: %s\n", SundaySchedules[idxCtr01].SecondReader);
            printf("\t\tPrayers of the Faithful (POF): %s\n", SundaySchedules[idxCtr01].POF);
            printf("\t\tCommentator: %s\n\n", SundaySchedules[idxCtr01].Commentator);
            printf("\n-----------------------------------------------------------------------------------------------\n\n");
            break;
        }
    }
    
    // If the schedule does not exist, then perform the code below 
    if(ministryMemberCount != 0){
	    printf("\tDesignate "); 
	    PrintSundayTimeSlot(inputTimeSlot);
	    printf(" Servers for ");
	    PrintMonth(inputMonth);
	    printf(" %d, %d\n", inputSunday, currentYear);
	
	    printf("\t\tInput [-1] or any negative number if no server will be assigned.\n");
	    printf("\t\tInput [0] to go back to Access Sunday Schedules Menu.\n");
	
	    InputServers(&serverOne, &serverTwo, &serverThree, &serverFour);    
	
	    // Resizes the dynamic array to store more schedule entries
	    sundayScheduleCount++;
	    SundaySchedules = (HolyMass *)realloc(SundaySchedules, sizeof(HolyMass) * sundayScheduleCount);
	
	    // Stores the inputs if the input is not equal to 5.
	    areSlotsScheduled = !SundaySchedules[sundayScheduleCount].isScheduled[0] ||
	                        !SundaySchedules[sundayScheduleCount].isScheduled[1] ||
	                        !SundaySchedules[sundayScheduleCount].isScheduled[2] ||
	                        !SundaySchedules[sundayScheduleCount].isScheduled[3];
	
	    if(areSlotsScheduled){
	        AssignServer(currentYear, inputMonth, inputSunday, inputTimeSlot, sundayScheduleCount - 1, serverOne, 0);
	        AssignServer(currentYear, inputMonth, inputSunday, inputTimeSlot, sundayScheduleCount - 1, serverTwo, 1);
	        AssignServer(currentYear, inputMonth, inputSunday, inputTimeSlot, sundayScheduleCount - 1, serverThree, 2);
	        AssignServer(currentYear, inputMonth, inputSunday, inputTimeSlot, sundayScheduleCount - 1, serverFour, 3);
	    }
	
	    else{
	        sundayScheduleCount--;
	        SundaySchedules = (HolyMass *)realloc(SundaySchedules, sizeof(HolyMass) * sundayScheduleCount);
	    }
	}
	
	else{
		printf("\tThere are no existing members stored in the program database. You will be redirected to the Main Menu. ");
		ExecuteProgram();
		ExitProgram(0);
	}
    
}

int isSunday(int year, int month, int day) {
    // Create a tm structure and initialize it with the given date
    struct tm date = {0};
    date.tm_year = year - 1900; // Years since 1900
    date.tm_mon = month - 1;   // Months are 0-based
    date.tm_mday = day;

    // Convert the tm structure to time_t
    time_t time = mktime(&date);

    // Use localtime to get the day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    struct tm *localTime = localtime(&time);
    int dayOfWeek = localTime->tm_wday;

    // Check if it's a Sunday
    return (dayOfWeek == 0);
}


// Viewing Schedule Functions
void ViewSundaySchedule(){    
    printf("\tSUNDAY SCHEDULES\n\n");
    
    for(idxCtr01 = 0; idxCtr01 < sundayScheduleCount; idxCtr01++){
        printf("\t");
        PrintMonth(SundaySchedules[idxCtr01].Month);
        printf(" %d, %d\n", SundaySchedules[idxCtr01].Date, SundaySchedules[idxCtr01].Year);
        printf("\t\tFirst Reader: %s\n", SundaySchedules[idxCtr01].FirstReader);
        printf("\t\tSecond Reader: %s\n", SundaySchedules[idxCtr01].SecondReader);
        printf("\t\tPrayers of the Faithful: %s\n", SundaySchedules[idxCtr01].POF);
        printf("\t\tCommentator: %s\n\n", SundaySchedules[idxCtr01].Commentator);
    }
}

void AccessWeekdaysSchedules(){

}

int CheckForDaysInAMonth(int month, int currentYear) {
    switch (month) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            // Check for leap year
            return (currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0) ? 29 : 28;
        default:
            return 31;
    }
}

void PrintCalendar(int month, int currentYear){ // Needs More Explaining
    int daysInAMonth, day, weekday;

    // Get the number of days in the specified month
    daysInAMonth = CheckForDaysInAMonth(month, currentYear);

    // Get the current date
    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);

    // Set the day to 1st of the specified month
    current_time->tm_year = currentYear - 1900;
    current_time->tm_mon = month - 1;
    current_time->tm_mday = 1;
    current_time->tm_hour = 12; // Avoid issues with daylight saving time
    current_time->tm_min = 0;
    current_time->tm_sec = 0;
    current_time->tm_isdst = -1;

    // Convert struct tm to time_t
    t = mktime(current_time);

    // Get the weekday of the 1st day
    weekday = localtime(&t)->tm_wday;

    // Print the calendar
    switch (month) {
        case 1: printf("\n\n\t\t\t\t%-10s %11s %d\n", "JANUARY", " ",currentYear); break;
        case 2: printf("\n\n\t\t\t\t%-10s %11s %d\n", "FEBRUARY", " ", currentYear); break;
        case 3: printf("\n\n\t\t\t\t%-10s %11s %d\n", "MARCH", " ",currentYear); break;
        case 4: printf("\n\n\t\t\t\t%-10s %11s %d\n", "APRIL", " ",currentYear); break;
        case 5: printf("\n\n\t\t\t\t%-10s %11s %d\n", "MAY", " ",currentYear); break;
        case 6: printf("\n\n\t\t\t\t%-10s %11s %d\n", "JUNE", " ",currentYear); break;
        case 7: printf("\n\n\t\t\t\t%-10s %11s %d\n", "JULY", " ",currentYear); break;
        case 8: printf("\n\n\t\t\t\t%-10s %11s %d\n", "AUGUST", " ",currentYear); break;
        case 9: printf("\n\n\t\t\t\t%-10s %11s %d\n", "SEPTEMBER", " ",currentYear); break;
        case 10: printf("\n\n\t\t\t\t%-10s %11s %d\n", "OCTOBER", " ",currentYear); break;
        case 11: printf("\n\n\t\t\t\t%-10s %11s %d\n", "NOVEMBER", " ",currentYear); break;
        case 12: printf("\n\n\t\t\t\t%-10s %11s %d\n", "DECEMBER", " ",currentYear); break;
    }

    printf("\t\t\t\t---------------------------\n");
    printf("\t\t\t\tSun Mon Tue Wed Thu Fri Sat\n");
    printf("\t\t\t\t");
    // Print leading spaces
    for (idxCtr01 = 0; idxCtr01 < weekday; idxCtr01++) {
        printf("    ");
    }

    // Print the days
    for (day = 1; day <= daysInAMonth; day++) {
        printf("%3d ", day);
        if ((weekday + day) % 7 == 0 || day == daysInAMonth) {
            printf("\n\t\t\t\t");
        }
    }

    printf("\n");
}


void CheckForExit(int errorCode){

}

void ExitProgram(int errorCode){
	BubbleSort();
    ExportMinistryMembers();
    ExportSundaySchedules();
	exit(0);
}

void ExportMinistryMembers(){
    MinistryMembersDatabase = fopen("MinistryMembersDatabase.txt", "w");
    
    qsort(MinistryMembers, ministryMemberCount, sizeof(MinistryMember), compareIndices);

    for(idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01++){
        fprintf(MinistryMembersDatabase, "%d, ", MinistryMembers[idxCtr01].Index);
        fprintf(MinistryMembersDatabase, "%s, ", MinistryMembers[idxCtr01].Surname);
        fprintf(MinistryMembersDatabase, "%s, ", MinistryMembers[idxCtr01].FirstName);
        fprintf(MinistryMembersDatabase, "%s, ", MinistryMembers[idxCtr01].MiddleName);
        fprintf(MinistryMembersDatabase, "%s, ", MinistryMembers[idxCtr01].Nickname);
        fprintf(MinistryMembersDatabase, "%c, ", MinistryMembers[idxCtr01].Sex);
        fprintf(MinistryMembersDatabase, "%d, ", MinistryMembers[idxCtr01].YearMembership);
        fprintf(MinistryMembersDatabase, "%d, ", MinistryMembers[idxCtr01].MonthMembership);
        fprintf(MinistryMembersDatabase, "%d\n", MinistryMembers[idxCtr01].DayMembership);
    }
    
    fclose(MinistryMembersDatabase);
}

void ImportMinistryMembers(){
    idxCtr01 = 0;
	MinistryMembersDatabase = fopen("MinistryMembersDatabase.txt", "r");
	
    if(MinistryMembersDatabase == NULL)  fclose(MinistryMembersDatabase);
	else{
		while (fscanf(MinistryMembersDatabase, "%d, %[^,], %[^,], %[^,], %[^,], %c, %d, %d, %d\n", 
	        &MinistryMembers[idxCtr01].Index,
	        MinistryMembers[idxCtr01].Surname,
	        MinistryMembers[idxCtr01].FirstName,
	        MinistryMembers[idxCtr01].MiddleName,
	        MinistryMembers[idxCtr01].Nickname,
	        &MinistryMembers[idxCtr01].Sex,
	        &MinistryMembers[idxCtr01].YearMembership,
	        &MinistryMembers[idxCtr01].MonthMembership,
	        &MinistryMembers[idxCtr01].DayMembership) != EOF) {	        	
				idxCtr01++;	        	
    	}

    	ministryMemberCount = idxCtr01;
    	fclose(MinistryMembersDatabase);
	}   
}

void ExportSundaySchedules(){
	SundayScheduleDatabase = fopen("SundayScheduleDatabase.txt", "w");
	
	for(idxCtr01 = 0; idxCtr01 < sundayScheduleCount; idxCtr01++){
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].Year);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].Month);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].Date);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].MassNumber);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].isScheduled[0]);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].isScheduled[1]);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].isScheduled[2]);
        fprintf(SundayScheduleDatabase, "%d, ", SundaySchedules[idxCtr01].isScheduled[3]);
        fprintf(SundayScheduleDatabase, "%s, ", SundaySchedules[idxCtr01].FirstReader);
        fprintf(SundayScheduleDatabase, "%s, ", SundaySchedules[idxCtr01].SecondReader);        
        fprintf(SundayScheduleDatabase, "%s, ", SundaySchedules[idxCtr01].POF);        
        fprintf(SundayScheduleDatabase, "%s\n", SundaySchedules[idxCtr01].Commentator);
    }
    
    fclose(SundayScheduleDatabase);
}

void ImportSundaySchedules(){
	idxCtr01 = 0;
	SundayScheduleDatabase = fopen("SundayScheduleDatabase.txt", "r");
	
	if(SundayScheduleDatabase == NULL)  fclose(SundayScheduleDatabase);
	else{		
    	SundaySchedules = (HolyMass *)realloc(SundaySchedules, (sundayScheduleCount + 1) * sizeof(HolyMass));
		
		while (fscanf(SundayScheduleDatabase, "%d, %d, %d, %d, %d, %d, %d, %d, %[^,], %[^,], %[^,], %[^,]\n", 
	        &SundaySchedules[idxCtr01].Year,
	        &SundaySchedules[idxCtr01].Month,
	        &SundaySchedules[idxCtr01].Date,
	        &SundaySchedules[idxCtr01].MassNumber,
	        &SundaySchedules[idxCtr01].isScheduled[0],
	        &SundaySchedules[idxCtr01].isScheduled[1],
	        &SundaySchedules[idxCtr01].isScheduled[2],
	        &SundaySchedules[idxCtr01].isScheduled[3],
	        SundaySchedules[idxCtr01].FirstReader,
			SundaySchedules[idxCtr01].SecondReader,
			SundaySchedules[idxCtr01].POF,
			SundaySchedules[idxCtr01].Commentator) != EOF) {	        	
				idxCtr01++;
				SundaySchedules = (HolyMass *)realloc(SundaySchedules, (sundayScheduleCount + 1) * sizeof(HolyMass));	        	
    	}

    	sundayScheduleCount = idxCtr01;
    	fclose(SundayScheduleDatabase);
	}	
}

void ClearInputBuffer(){
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}
