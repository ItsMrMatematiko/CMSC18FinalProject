/* 
    Project Title:
        Church Lectors' and Commentators' Ministry Scheduling System

    Project Proponents: 
        ARRANGUEZ, Francis Reid N.
        CASQUEJO, Jann Dave Rhodore G.
        MORGIA, Maron Christofer E.
        RULIDA, Marvie Joyce D.

    Project Completion Date: 
        15 January 2023
*/

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
void ExecuteProgram();
int MainMenu();
void AccessMinistryMembers();
void AddRecord();
void DeleteMinistryMember();
void CheckNewRecordEqualsSignChar(char *);
void CheckNewRecordEqualsSignInt(int);
void CheckNewRecordEqualsSignChar2(char);
void BubbleSort();
void YearSort();
void GenderSort();
void BubbleSortGender(int);
void DisplayAll();
void PrintMembersWithNumberAndNickname();
int compareIndices(const void *a, const void *b);
int GetSelectedMonth();
int GetSelectedDate(int month);
void AssignMonth(int, char *);
void PrintMonth(int);
int AskServerIndex(int *indexInput);
void CheckServerInputZero(int *);
void AssignServer(HolyMass *scheduleDatabase, int, int, int, int, int, int, int);
void InputServers(int *, int *, int *, int *);
void RedesignateServers(HolyMass *scheduleDatabase, int);
void DesignateNewServers(HolyMass *scheduleDatabase, int, int, int, int, int, int *, int *, int *, int *);
int isSunday(int, int, int);
int isMonday(int, int, int);
int compareSundaySchedules(const void *a, const void *b);
int CheckForDaysInAMonth(int, int);
void PrintCalendar(int, int);
void AccessSundaysSchedules();
void PrintSundayTimeSlot(int);
void AddSundaySchedule();
void AccessSundaysSchedules();
void PrintSundayTimeSlot(int);
void DesignateNewServers(HolyMass *scheduleDatabase, int, int, int, int, int, int *, int *, int *, int *);
void RedesignateServers(HolyMass *scheduleDatabase, int);
void AccessWeekdaysSchedules();
void PrintWeekdayTimeSlot(int);
void AddWeekdaySchedule();
void ViewSchedule(int);
void ExportMinistryMembers();
void ImportMinistryMembers();
void ExportSchedules(HolyMass *scheduleDatabase, int *, int);
void ImportSchedules(HolyMass **scheduleDatabase, int *, char *);
void ClearInputBuffer();
void FinalPrint();
void ExitProgram(int);



int main(){    
    // This is the main program where the essential program functions are called.
    // It is expected to do the following:
    //      [1] Allocate Memory for the Dynamic Arrays while importing the database contents
    //          from the .txt files to their corresponding dynamic arrays
    //      [2] Call the execution of the program functionalities	
    
    // This code line imports the stored MinistryMember records into the MinistryMembers dynamic array.
    ImportMinistryMembers(); 

    // These next two code lines import the stored schedules from both the SundaySchedules and 
    // WeekdaySchedules .txt files to their corresponding databases.
	ImportSchedules(&SundaySchedules, &sundayScheduleCount, "SundayScheduleDatabase.txt"); // Import existing Ministry Members data.
    ImportSchedules(&WeekdaySchedules, &weekdayScheduleCount, "WeekdayScheduleDatabase.txt"); // Import existing Sunday schedules. 
    
    // This code line calls the rest of the program functionalities and executes the program.
    ExecuteProgram();

    // This is an expected return value as the main() function is set to return an integer value.
    return 0;
}

// Function to print out the main header of the program.
void PrintAppHeader(){
    system("cls");
    printf("===============================================================================================\n");
    printf("OUR LADY OF FATIMA PARISH\n");
    printf("Fatima St., Brgy. 24-C, Davao City, Philippines, 8000\n");
    printf("Lectors' and Commentators' Ministry\n");
    printf("DIGITAL SERVICE SCHEDULING APPLICATION\n");
    printf("===============================================================================================\n");
}

// Function to execute the main program logic.
void ExecuteProgram(){	
    if (ministryMemberCount > 0) BubbleSort(); // Sort MinistryMembers if there are members.    
    PrintAppHeader();

    int choiceOfAction = MainMenu(); // Display and get user input from the main menu.

    // Execute the selected action based on user input.
    switch(choiceOfAction){
        case 1 : AccessMinistryMembers(); break;
        case 2 : AccessWeekdaysSchedules(); break;            
        case 3 : AccessSundaysSchedules(); break;            
        case 0 : ExitProgram(0); break;
    }

    // Check if the user wants to perform other actions.
    FinalPrint();
}

// This function holds the main menu of the program.
// Serves as the main selection function to access different functions.
int MainMenu(){    
    int selectedOption;

    printf("\n\tPROGRAM OPTIONS\n\n");
    printf("\t\t[1] Access Ministry Members\n");
    printf("\t\t[2] Access Weekdays Scheduled Servers\n");
    printf("\t\t[3] Access Sundays Scheduled Servers\n");
    printf("\t\t[0] Exit the Program\n\n");
        
    do{
        printf("\tWhich of the following actions would you like to do? ");
        scanf(" %d", &selectedOption);
        ClearInputBuffer();          

        if(selectedOption != 1 && selectedOption != 2 && selectedOption != 3 && selectedOption != 0){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }        
    }while(selectedOption != 1 && selectedOption != 2 && selectedOption != 3 && selectedOption != 0);   
        
    return selectedOption;
}

// Function to record Ministry Members.
// Main menu for Ministry Member data manipulation.
void AccessMinistryMembers(){
    PrintAppHeader();
    
	// Do while loop to ask choice of user and makes sure that only the given choices are inputted.
    do {
        printf("\n\tPROGRAM OPTIONS\n\n");
        printf("\t\t[1] Add New Records\n");
        printf("\t\t[2] View All Existing Records\n");
        printf("\t\t[3] Delete a Member\n");
        printf("\t\t[0] Go Back to Main Menu\n\n");
        
        printf("\tEnter Choice here: ");
        scanf(" %d", &choice);
        ClearInputBuffer();  
            
        if(choice != 1 && choice != 2 && choice != 3 && choice != 0){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }   
    } while (choice != 1 && choice != 2 && choice != 3 && choice != 0);

	// Switch case function to analyze user input and execute different functions.
    switch(choice){
        case 1:
            AddRecord();
            BubbleSort();
            break;

        case 2:
            DisplayAll();
            break;

        case 3:
            DeleteMinistryMember();
            break;

        case 0:
            ExecuteProgram();
            ExitProgram(0);
            break;

        default:
            break;
    } 
}

// Function for adding Ministry Member data into database.
void AddRecord(){
	char tempSurname[21], tempFirstName[31], tempMiddleName[16], tempNickname[16], tempSex;
	int tempYear, tempMonth, tempDay, nicknameExists = 0, daysInMonth;
	
	// If function to make sure ministry member input does not exceed max storage capacity of MinistryMembers struct.
    if (ministryMemberCount < MAX)
    {
    	PrintAppHeader();
        printf("\n\t-----------------------------------------------------------------");
        printf("\n\n");
        printf("\tIf you wish to return to the previous section please enter: [-1]\n");
        printf("\tIf you wish to return to the Main Menu please enter: [0]\n\n");
            
        printf("\tEnter Member's Surname: "); // User input for member surname.
        scanf(" %[^\n]s", &tempSurname);
        CheckNewRecordEqualsSignChar(tempSurname); 
        ClearInputBuffer();  
		
        printf("\tEnter Member's First Name: "); // User input for member first name.
        scanf(" %[^\n]s", &tempFirstName);
        CheckNewRecordEqualsSignChar(tempFirstName);  
        ClearInputBuffer();  
		    
        printf("\tEnter Member's Middle Name: "); // User input for member middle name.
        scanf(" %[^\n]s", &tempMiddleName);
        CheckNewRecordEqualsSignChar(tempMiddleName);  
        ClearInputBuffer();  
        
        do {
        	nicknameExists = 0;
        	
            printf("\tEnter Member's Nickname: "); // User input for member nick name.
            scanf(" %[^\n]s", &tempNickname);
            CheckNewRecordEqualsSignChar(tempNickname);  
            ClearInputBuffer();
			
			// For loop to make sure that nickname input does not yet exist.
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
        
		// Do-while loop to make sure that user input is only equal to m, M, f, or F.    
        do {    
    		printf("\tSex (M or F): ");
    		scanf(" %c", &tempSex);
    		ClearInputBuffer();

    		// Check for special cases
    		if (tempSex == '0') {
    		    ExecuteProgram();
    		    ExitProgram(0);
    		} else if (tempSex == '-') {
    		    AccessMinistryMembers();
    		    FinalPrint();
    		}
			
			tempSex = toupper(tempSex);
    		
    		if (tempSex != 'M' && tempSex != 'F') {
    		    printf("\t\tInvalid input, please choose between M or F. Or enter -1 or 0.\n");
    		}

    			
			} while (tempSex != 'M' && tempSex != 'F'); 
        
        // Do while loop to ensure proper timeline of church
		do {    
            printf("\tEnter year of Membership: "); // User input for member's year of membership
            scanf("%d", &tempYear);
            CheckNewRecordEqualsSignInt(tempYear);
                
            if (tempYear < 1952) 
                printf("\t\tInvalid year. The church has not yet existed at this time.\n");

            else if (tempYear > 2024)
                printf("\t\tInvalid year. Please enter a year between 1952 and 2024.\n");

            else 
                break;
        } while (tempYear < 1952 || tempYear > 2024);
        
		// Do while loop to ensure user input is along the range of months in a year.    
        do {
            printf("\tEnter Month of Membership (1-12): ");  // User input for member's month of membership
            scanf("%d", &tempMonth);
            CheckNewRecordEqualsSignInt(tempMonth);
            ClearInputBuffer();  
            if (1 > tempMonth || 12 < tempMonth)
                printf("\t\tInvalid Month please choose between 1-12\n");

            else
                break;

        } while (1 > tempMonth || 12 < tempMonth);
        
		// Do while loop to ensure user input for day corresponds with the year and month previously inputted.
        do {
    		printf("\tEnter day of Membership: ");
    		scanf("%d", &tempDay);
    		CheckNewRecordEqualsSignInt(tempDay); 
    		ClearInputBuffer();
			
			// Calls to function CheckForDaysInAMonth
    		daysInMonth = CheckForDaysInAMonth(tempMonth, tempYear);

    		if (tempDay < 1 || tempDay > daysInMonth)
        		printf("\t\tInvalid day for the selected month. Please try again.\n");
    		
            else
        	    break;
        } while (tempDay < 1 || tempDay > daysInMonth);

		// Reassigns variables to MinistryMembers instances according to current count.
		MinistryMembers[ministryMemberCount].Index = ministryMemberCount;
		strcpy(MinistryMembers[ministryMemberCount].Surname, tempSurname);
		strcpy(MinistryMembers[ministryMemberCount].FirstName, tempFirstName);
		strcpy(MinistryMembers[ministryMemberCount].MiddleName, tempMiddleName);
		strcpy(MinistryMembers[ministryMemberCount].Nickname, tempNickname);
		MinistryMembers[ministryMemberCount].Sex = tempSex;
		MinistryMembers[ministryMemberCount].YearMembership = tempYear;
		MinistryMembers[ministryMemberCount].MonthMembership = tempMonth;
		MinistryMembers[ministryMemberCount].DayMembership = tempDay;
        ministryMemberCount++; // Increments total count for ministry members.
            
        }
        else    
            printf("\t\tAction declined. Maximum amount of records acquired.");
    
    
    printf("\n");
    printf("\t-----------------------------------------------------------------\n\n");

}

// Function for erasing Ministry Member in database.
void DeleteMinistryMember(){
    int DeletedMember;
    PrintAppHeader();
    DisplayAll(); // Calls to function DisplayAll to display all curent members in database.

    printf("\n\tEnter the number of the member you want to delete (0 to cancel): "); 
    scanf("%d", &DeletedMember);
    ClearInputBuffer();
	
	// Check if the entered member number is within valid range
    if (DeletedMember >= 1 && DeletedMember <= ministryMemberCount) {
    	
    	// Loop to shift array elements to fill the gap left by the deleted member
        for (idxCtr01 = DeletedMember - 1; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
            MinistryMembers[idxCtr01] = MinistryMembers[idxCtr01 + 1];
            MinistryMembers[idxCtr01].Index = idxCtr01 + 1; // Update the index of the moved member
        }

        ministryMemberCount--; // Reduce the count of members after deletion
        printf("\n\tMember deleted successfully.\n");
    } else if (DeletedMember != 0) {
    	// If the entered member number is not in the valid range
        printf("\n\tInvalid member number. No changes made.\n");
    }

    printf("\n\t-----------------------------------------------------------------\n\n");
}

// Checks user input for '-1' or '0' to either return to the previous section or return to Main Menu (string variables).
void CheckNewRecordEqualsSignChar(char* temp){
    if (strcmp(temp, "-1") == 0) {
    	AccessMinistryMembers();
    	FinalPrint();
    }
    else if (strcmp(temp, "0") == 0) {
        ExecuteProgram();
        ExitProgram(0);
    }
}

// Checks user input for '-1' or '0' to either return to the previous section or return to Main Menu (integer variables).
void CheckNewRecordEqualsSignInt(int temp){
    if (temp == 0) {
        ExecuteProgram();
        ExitProgram(0);
    }
    if (temp == -1) {
        AccessMinistryMembers();
        FinalPrint();
    }
}

// Checks user input for '=' or '0' to either return to the previous section or return to Main Menu (character variables).
void CheckNewRecordEqualsSignChar2(char temp){
    if (temp == '0') {
        ExecuteProgram();
        ExitProgram(0);
    }
}
  

// BubbleSort function to sort MinistryMembers array based on Surname
void BubbleSort() {
    // Outer loop for multiple passes through the array
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        // Inner loop for comparing and swapping elements
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            // Compare surnames (case-insensitive) using strcmp and strupr
            if (strcmp(strupr(MinistryMembers[idxCtr01].Surname), strupr(MinistryMembers[idxCtr02].Surname)) > 0) {
                // Swap MinistryMembers[idxCtr01] and MinistryMembers[idxCtr02]
                MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                // Swap the index values as well to maintain correct order
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}

// Function to sort Ministry Members by their membership year in descending order.
void YearSort(){
    // Outer loop to iterate through each member in the MinistryMembers array.
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        // Inner loop to compare the current member with the subsequent members.
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            // Check if the membership year of the current member is less than the next member.
            if (MinistryMembers[idxCtr01].YearMembership < MinistryMembers[idxCtr02].YearMembership) {
                // Swap the positions of the current member and the next member.
                MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                // Swap the index values of the members for correct indexing after sorting.
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}

// Void function to sort Ministry Members by gender.
void GenderSort(){
	PrintAppHeader();
	do{
    printf("\n\tSort by Gender:\n");
    printf("\t\t[1] Male first\n");
    printf("\t\t[2] Female first\n");
    printf("\t\t[0] Return to Main Menu\n");
    printf("\t\t[-1] Return to previous section\n\n");

    printf("\tEnter your choice: ");
    scanf("%d", &genderoption);
    ClearInputBuffer(); 
    
    if (genderoption == 0){
		ExecuteProgram();
		ExitProgram(0);
	}
	else if (genderoption == -1){
		DisplayAll();
		FinalPrint();
	}
	
    if (genderoption != 2 && genderoption != 1 && genderoption != 0 && genderoption !=-1){
    	printf("\t\t\t Invalid input. Please try again");
	}
    }while (genderoption != 2 && genderoption != 1 && genderoption != 0 && genderoption !=-1);
     
        
    BubbleSortGender(genderoption);
}

// Function to perform bubble sort based on gender option.
void BubbleSortGender(int genderoption) {
    // Iterate through each member using two nested loops.
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount - 1; idxCtr01++) {
        for (idxCtr02 = idxCtr01 + 1; idxCtr02 < ministryMemberCount; idxCtr02++) {
            // Check the gender option and conditions for swapping.
            if ((genderoption == 1 && MinistryMembers[idxCtr01].Sex == 'F' && MinistryMembers[idxCtr02].Sex != 'F') ||
                (genderoption == 2 && MinistryMembers[idxCtr01].Sex == 'M' && MinistryMembers[idxCtr02].Sex == 'F')) {
                // Swap positions of the current member and the next member.
                struct MinistryMember temp = MinistryMembers[idxCtr01];
                MinistryMembers[idxCtr01] = MinistryMembers[idxCtr02];
                MinistryMembers[idxCtr02] = temp;
                
                // Swap the index values of the members for correct indexing after sorting.
                int tempIndex = MinistryMembers[idxCtr01].Index;
                MinistryMembers[idxCtr01].Index = MinistryMembers[idxCtr02].Index;
                MinistryMembers[idxCtr02].Index = tempIndex;
            }
        }
    }
}


// Function to display all ministry members based on user's choice of sorting.
void DisplayAll() {
    char monthName[10];
    
    PrintAppHeader();

    // Loop to get the user's choice for sorting.
    do {
        printf("\n\tHow do you wish to view the record?\n\n");
        printf("\t\t[1] Default (Ascending Alphabetical Order)\n");
        printf("\t\t[2] Year Sort (Recent Members)\n");
        printf("\t\t[3] Gender Sort\n");
        printf("\t\t[0] Return to Main Menu\n");
        printf("\t\t[-1] Return to previous section\n\n");

        printf("\tInput Choice: ");
        scanf("%d", &option);
        ClearInputBuffer();

        // Validate user input.
        if (option != 3 && option != 2 && option != 1 && option != 0 && option != -1)
            printf("\t\tInvalid input. Please try again\n");
        else
            break;
    } while (option != 3 && option != 2 && option != 1 && option != 0 && option != -1);

    // Perform sorting based on user's choice.
    switch (option) {
        case 1: BubbleSort(); break;
        case 2: YearSort(); break;
        case 3: GenderSort(); break;
        case -1: AccessMinistryMembers(); FinalPrint(0);
        case 0: ExecuteProgram(); ExitProgram(0);
        default: printf("\tInvalid option\n");
    }

    // Print the application header and display the ministry member database.
    PrintAppHeader();
    printf("\t|=========================================================================|\n");
    printf("\t|                             MEMBER DATABASE                             |\n");
    printf("\t|-------------------------------------------------------------------------|\n");

    // Loop to display each member's information.
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01++) {
        // Assign the month name based on the month number.
        AssignMonth(MinistryMembers[idxCtr01].MonthMembership, monthName);

        // Display member information in a formatted manner.
        printf("\t| %2d |  Surname: %-35s   | Sex: %-9c   |\n", idxCtr01 + 1, MinistryMembers[idxCtr01].Surname, MinistryMembers[idxCtr01].Sex);
        printf("\t|    |  First Name: %-32s   | Year: %04d       |\n", MinistryMembers[idxCtr01].FirstName, MinistryMembers[idxCtr01].YearMembership);
        printf("\t|    |  Middle Name: %-32s  | Month: %-9s |\n", MinistryMembers[idxCtr01].MiddleName, monthName);
        printf("\t|    |  Nickname: %-33s    | Day: %02d          |\n", MinistryMembers[idxCtr01].Nickname, MinistryMembers[idxCtr01].DayMembership);
        printf("\t|-------------------------------------------------------------------------|\n");
    }

    printf("\t====================================================================================\n\n");
}

// Function to print ministry members with their numbers, surnames, and nicknames in two columns.
void PrintMembersWithNumberAndNickname() {
    printf("\tEXISTING MINISTRY MEMBERS\n\n");

    printf("\t===============================================\t\t===============================================\n");
    printf("\t| No. | Surname            | Nickname         |\t\t| No. | Surname            | Nickname         |\n");
    printf("\t|-----|--------------------|------------------|\t\t|-----|--------------------|------------------|\n");

    // Print two tables side by side
    for (idxCtr01 = 0; idxCtr01 < ministryMemberCount; idxCtr01 += 2) {
        printf("\t| %-3d | %-18s | %-16s |\t\t", idxCtr01 + 1, MinistryMembers[idxCtr01].Surname, MinistryMembers[idxCtr01].Nickname);

        // Check if there is a second member in the current row
        if (idxCtr01 + 1 < ministryMemberCount) {
            printf("| %-3d | %-18s | %-16s |\n", idxCtr01 + 2, MinistryMembers[idxCtr01 + 1].Surname, MinistryMembers[idxCtr01 + 1].Nickname);
        }

        else printf("===============================================\n");
    }

    printf("\t===============================================\n");
}

// Function to compare two MinistryMember structures based on their Index values.
// This function is designed to be used with the qsort function for sorting.
int compareIndices(const void *a, const void *b) {
    // Convert void pointers to MinistryMember pointers for easy access to structure members.
    const MinistryMember *ma = (const MinistryMember *)a;
    const MinistryMember *mb = (const MinistryMember *)b;

    // Compare the Index values of the two MinistryMember structures.
    if ((*ma).Index < (*mb).Index) return -1;
    else if ((*ma).Index > (*mb).Index) return 1;
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
    printf("\t[0] Go Back to Main Menu\n");
    printf("\t[-1] Return to previous section\n\n");
    
    do{
        printf("\tEnter the Month to be placed with a Schedule (Only for Year %d) [1 - 12]: ", currentYear);
        scanf(" %d", &input);
        ClearInputBuffer();
        
        if(input < -1 || input > 12) printf("\t\t\tYou provided an invalid input. Please try again.\n\n");
        else return input;
    }while(input < -1 || input > 12);
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
    CheckServerInputZero(indexInput);

    if(*indexInput > ministryMemberCount){
        printf("\t\tYou provided an input beyond the number of members. Please try again.\n\n");
    }

    else return *indexInput;
}

void CheckServerInputZero(int *serverIndex){
    if(*serverIndex == 0){
        system("cls");
        ExecuteProgram();
        ExitProgram(0);
    }
}

void AssignServer(HolyMass *scheduleDatabase, int scheduleIndex, int year, int month, int date, int timeSlotIndex, int ministryMemberIndex, int servicePosition){    
	if(ministryMemberIndex >= 0){
        switch(servicePosition){
            case 0:                
				strcpy(scheduleDatabase[scheduleIndex].FirstReader, MinistryMembers[ministryMemberIndex].Nickname);
                scheduleDatabase[scheduleIndex].isScheduled[0] = 1;                
                break;
            case 1:
				strcpy(scheduleDatabase[scheduleIndex].SecondReader, MinistryMembers[ministryMemberIndex].Nickname);
                scheduleDatabase[scheduleIndex].isScheduled[1] = 1; 
                break;
            case 2:
                strcpy(scheduleDatabase[scheduleIndex].POF, MinistryMembers[ministryMemberIndex].Nickname);
                scheduleDatabase[scheduleIndex].isScheduled[2] = 1; 
                break;
            case 3:
                strcpy(scheduleDatabase[scheduleIndex].Commentator, MinistryMembers[ministryMemberIndex].Nickname);
                scheduleDatabase[scheduleIndex].isScheduled[3] = 1; 
                break;
        }
    }    

    else{
        switch(servicePosition){
            case 0: 
                strcpy(scheduleDatabase[scheduleIndex].FirstReader, "NONE"); 
                scheduleDatabase[scheduleIndex].isScheduled[0] = 0; 
                break;
            case 1: 
                strcpy(scheduleDatabase[scheduleIndex].SecondReader, "NONE"); 
                scheduleDatabase[scheduleIndex].isScheduled[1] = 0; 
                break;
            case 2: 
                strcpy(scheduleDatabase[scheduleIndex].POF, "NONE"); 
                scheduleDatabase[scheduleIndex].isScheduled[2] = 0; 
                break;
            case 3: 
                strcpy(scheduleDatabase[scheduleIndex].Commentator, "NONE"); 
                scheduleDatabase[scheduleIndex].isScheduled[3] = 0; 
                break;
        }        
    }
    
    scheduleDatabase[scheduleIndex].Year = year;
    scheduleDatabase[scheduleIndex].Month = month;
    scheduleDatabase[scheduleIndex].Date = date;
    scheduleDatabase[scheduleIndex].MassNumber = timeSlotIndex;
}

void InputServers(int *serverOne, int *serverTwo, int *serverThree, int *serverFour){
    // Designate Servers
        do{
            printf("\t First Reader: ");
            AskServerIndex(serverOne);
        }while(*serverOne > ministryMemberCount);    

        do{
            printf("\t Second Reader: ");
            AskServerIndex(serverTwo);

            if(*serverOne == *serverTwo && *serverTwo > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverTwo > ministryMemberCount || (*serverOne == *serverTwo && *serverTwo > 0));
        
        do{
            printf("\t Prayers of the Faithful (POF): ");
            AskServerIndex(serverThree);

            if((*serverOne == *serverThree || *serverTwo == *serverThree) && *serverThree > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverThree > ministryMemberCount || ((*serverOne == *serverThree || *serverTwo == *serverThree) && *serverThree > 0));

        do{
            printf("\t Commentator: ");
            AskServerIndex(serverFour);

            if((*serverOne == *serverFour || *serverTwo == *serverFour || *serverThree == *serverFour) && *serverFour > 0){
                printf("\t\t You have inputted a duplicate server. Please try again.\n\n");
            }
        }while(*serverThree > ministryMemberCount || (*serverOne == *serverFour || *serverTwo == *serverFour || *serverThree == *serverFour) && *serverFour > 0);
}

void RedesignateServers(HolyMass *scheduleDatabase, int scheduleDatabaseIndex){
    int year = scheduleDatabase[scheduleDatabaseIndex].Year;
    int month = scheduleDatabase[scheduleDatabaseIndex].Month;
    int day = scheduleDatabase[scheduleDatabaseIndex].Date;
    int timeSlot = scheduleDatabase[scheduleDatabaseIndex].MassNumber;

    int firstServer, secondServer, thirdServer, fourthServer;
    
    if(ministryMemberCount > 0){
        printf("\tRedesignate All Servers for "); 
	    PrintMonth(month);
	    printf(" %d, %d", day, year);
        printf(" at [");
        PrintSundayTimeSlot(timeSlot);
        printf("]\n\n");

        printf("\t If you wish to keep the same servers for a position, simply reinput their index number as shown above.\n");

        printf("\t\tInput [-1] or any negative number if no server will be assigned.\n");
	    printf("\t\tInput [0] to go back to Access Sunday Schedules Menu.\n\n");

        InputServers(&firstServer, &secondServer, &thirdServer, &fourthServer);

        AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, firstServer - 1, 0);
        AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, secondServer - 1, 1);
        AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, thirdServer - 1 , 2);
        AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, fourthServer - 1, 3);
    }

    else{
		printf("\tThere are no existing members stored in the program database. You will be redirected to the Main Menu. ");
		ExecuteProgram();
		ExitProgram(0);
	}
}

void DesignateNewServers(HolyMass *scheduleDatabase, int scheduleDatabaseIndex, int year, int month, int day, int timeSlot, int *firstServer, int *secondServer, int *thirdServer, int *fourthServer){

    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    
    if(ministryMemberCount != 0){
	    printf("\tDesignate Servers for "); 
	    PrintMonth(month);
	    printf(" %d, %d", day, year);
        printf(" at [");
            if(!isSunday(year, month, day)) PrintWeekdayTimeSlot(timeSlot);
            else PrintSundayTimeSlot(timeSlot);
        printf("]\n");
	
	    printf("\t\tInput [-1] or any negative number if no server will be assigned.\n");
	    printf("\t\tInput [0] to go back to Main Menu.\n\n");
	
	    InputServers(firstServer, secondServer, thirdServer, fourthServer);

	    AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, *firstServer - 1, 0);
	    AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, *secondServer - 1, 1);
	    AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, *thirdServer - 1, 2);
	    AssignServer(scheduleDatabase, scheduleDatabaseIndex, year, month, day, timeSlot, *fourthServer - 1, 3);
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

int isMonday(int year, int month, int day) {
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

    // Check if it's a Monday
    return (dayOfWeek == 1);
}

int compareSundaySchedules(const void *a, const void *b) {
    const HolyMass *scheduleA = (const HolyMass *)a;
    const HolyMass *scheduleB = (const HolyMass *)b;

    if ((*scheduleA).Year != (*scheduleB).Year) return (*scheduleA).Year - (*scheduleB).Year;
    if ((*scheduleA).Month != (*scheduleB).Month) return (*scheduleA).Month - (*scheduleB).Month;
    if ((*scheduleA).Date != (*scheduleB).Date) return (*scheduleA).Date - (*scheduleB).Date;
    if ((*scheduleA).MassNumber != (*scheduleB).MassNumber) return (*scheduleA).MassNumber - (*scheduleB).MassNumber;

    return 0;
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

// Sunday Scheduling Functions
void AccessSundaysSchedules(){
    int selectedOption;

    PrintAppHeader();
    printf("\n\tSUNDAY SCHEDULES\n\n");
    printf("\t\t[1] Add/Modify Server Schedules for a Specific Sunday\n");
    printf("\t\t[2] View Scheduled Sunday Servers\n");
    printf("\t\t[0] Go Back to Main Menu\n");
    
    do{
        printf("\n\tWhich of the following actions would you like to do? ");
        scanf(" %d", &selectedOption);
        ClearInputBuffer();

        if(selectedOption != 1 && selectedOption != 2 && selectedOption != 0){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }           
    }while(selectedOption != 1 && selectedOption != 2 && selectedOption != 0);

    switch(selectedOption){
        case 1: AddSundaySchedule(); break;
        case 2: ViewSchedule(1); break; 
        case 0: ExecuteProgram(); break;
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

void AddSundaySchedule(){
    int inputMonth, inputSunday, inputTimeSlot, toAssign = 0, dateHasSchedule, scheduleEntryMatch;
    int serverOne, serverTwo, serverThree, serverFour;
    
    // The next 5 lines of a code block are preliminary codes for the program access the current time.
    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;
    
    PrintAppHeader();
    printf("\tSUNDAY SERVICE SCHEDULING\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // This code block will ask the user to select a month that would be scheduled.
    // Only the months in year 2024 will be scheduled.
    inputMonth = GetSelectedMonth();
    
    if(inputMonth == -1){
        system("cls");
        AccessSundaysSchedules();
        FinalPrint();
    }
    
    else if(inputMonth == 0){
    	ExecuteProgram();
	}
    
    // This line will call the function to print a calendar that can be viewed by the user.
    PrintCalendar(inputMonth, currentYear);

    // This do-while loop will ask the user to select a day that would be given a schedule.
    // This loop will handle errors concerning the date inputs: be it a past date, an invalid date, or a non-Sunday date.
    do{
        inputSunday = GetSelectedDate(inputMonth);

        if(inputSunday == -1){
        system("cls");
        AccessSundaysSchedules();
        FinalPrint();
    	}
    
    	else if(inputSunday == 0){
    	ExecuteProgram();
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
    printf("\t\t[0] Go back to Main Menu\n");
    printf("\t\t[-1] Go back to Access Sunday Schedules Menu\n\n");

    // This do-while loop will ask the user to select a time slot for a specific Sunday and check if it is a valid input.
    do{
        printf("\tSelected Time Slot: ");
        scanf(" %d", &inputTimeSlot);
        ClearInputBuffer();

        if(inputTimeSlot < -1 || inputTimeSlot > 6){
            printf("\t\t\tYou provided an invalid input. Please try again.\n");
        }		

        else if(inputTimeSlot == -1){
            system("cls");
            AccessSundaysSchedules();
            FinalPrint();
        }
        
        else if(inputMonth == 0){
        	ExecuteProgram;
    	}
    }while(inputTimeSlot < 1 || inputTimeSlot > 6);
    
    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    // This line will call the function that would print the index of the existing members alongside their nicknames.
    PrintMembersWithNumberAndNickname();

    printf("\n-----------------------------------------------------------------------------------------------\n");

    // This code block (up until the end of this function) will check for the existing schedules in the SundaySchedules dynamic array.
    // If there is an existing schedule, then it will be open for modifications.
    // If there are no existing schedules, then it will be created and assigned accordingly.
    dateHasSchedule = 0; 
    scheduleEntryMatch = 0;

    for(idxCtr01 = 0; idxCtr01 < sundayScheduleCount; idxCtr01++){
        // This value assignment will check for an exact match of an existing schedule in the SundaySchedules dynamic array.
        scheduleEntryMatch = SundaySchedules[idxCtr01].Year == currentYear &&
                             SundaySchedules[idxCtr01].Month == inputMonth &&
                             SundaySchedules[idxCtr01].Date == inputSunday &&
                             SundaySchedules[idxCtr01].MassNumber == inputTimeSlot;

        // This if condition checks if there is both an existing schedule in the record and a schedule assignment for the said schedule.
        // This code block shall cater to both assigned and marked as 'NONE' positions, provided that not all date entries
        // would exist prior to running this program.
        if(scheduleEntryMatch){
            // This code block will print the  date and time slot of the existing schedule
            printf("\n\tCurrent Servers for ");
            PrintMonth(inputMonth);
            printf(" %d, %d at [", inputSunday, currentYear);
            PrintSundayTimeSlot(inputTimeSlot);
            printf("]\n");

            // This code block will print the names of those assigned in a specific service schedule position.
            printf("\t\tFirst Reader: %s\n", SundaySchedules[idxCtr01].FirstReader);
            printf("\t\tSecond Reader: %s\n", SundaySchedules[idxCtr01].SecondReader);
            printf("\t\tPrayers of the Faithful (POF): %s\n", SundaySchedules[idxCtr01].POF);
            printf("\t\tCommentator: %s\n\n", SundaySchedules[idxCtr01].Commentator);
            printf("-----------------------------------------------------------------------------------------------\n");

            // This code line will call the function that allows the user to redesignate the schedules of the servers.
            RedesignateServers(SundaySchedules, idxCtr01);
            break;
        }
    }

    // Should the schedule date not yet exist in the SundaySchedules dynamic array, then this condition will be performed.
    if(!scheduleEntryMatch){
        // This line increments the number of schedules stored in the SundaySchedules dynamic array.
        sundayScheduleCount++; 
        
        // This line will reallocate memory to cater the new entry to be encoded to the SundaySchedules dynamic array.
	    SundaySchedules = (HolyMass *)realloc(SundaySchedules, sizeof(HolyMass) * sundayScheduleCount);

        // This code line will call the function that allows the user to add new server schedules into the SundaySchedules dynamic array.
        DesignateNewServers(SundaySchedules, sundayScheduleCount - 1, currentYear, inputMonth, inputSunday, inputTimeSlot, &serverOne, &serverTwo, &serverThree, &serverFour);
    }    
}

// Weekday Scheduling Functions
void AccessWeekdaysSchedules(){
   	int selectedOption;

    PrintAppHeader();
    printf("\n\tWEEKDAY SCHEDULES\n\n");
    printf("\t\t[1] Add/Modify Server Schedules for a Specific Weekday\n");
    printf("\t\t[2] View Scheduled Weekday Servers\n");
    printf("\t\t[0] Go Back to Main Menu\n");
    
    do{
        printf("\n\tWhich of the following actions would you like to do? ");
        scanf(" %d", &selectedOption);
        ClearInputBuffer();

        if(selectedOption != 1 && selectedOption != 2 && selectedOption != 0){
            printf("\t\tYou provided an incorrect input. Please try again.\n\n");
        }           
    }while(selectedOption != 1 && selectedOption != 2 && selectedOption != 0);

    switch(selectedOption){
        case 1: AddWeekdaySchedule(); break;
        case 2: ViewSchedule(0); break; 
        case 0: ExecuteProgram(); break;
    }  
}

void PrintWeekdayTimeSlot(int timeSlotCode){    
    switch(timeSlotCode){
        case 1: printf("06:00 AM"); break;
        case 2: printf("05:15 PM"); break;
    }    
}

void AddWeekdaySchedule(){
    int inputMonth, inputWeekday, inputTimeSlot, toAssign = 0, scheduleEntryMatch;
    int serverOne, serverTwo, serverThree, serverFour;
    
    // The next 5 lines of a code block are preliminary codes for the program access the current time.
    time_t t;
    struct tm* current_time;
    time(&t);
    current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;
    
    PrintAppHeader();
    printf("\tWEEKDAY SERVICE SCHEDULING\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // This code block will ask the user to select a month that would be scheduled.
    // Only the months in year 2024 will be scheduled.
    inputMonth = GetSelectedMonth();
    
    if(inputMonth == -1){
        system("cls");
        AccessWeekdaysSchedules();
        FinalPrint();
    }
    
    else if(inputMonth == 0){
    	ExecuteProgram();
	}
    
    // This line will call the function to print a calendar that can be viewed by the user.
    PrintCalendar(inputMonth, currentYear);

    // This do-while loop will ask the user to select a day that would be given a schedule.
    // This loop will handle errors concerning the date inputs: be it a past date, an invalid date, or a non-Sunday date.
    do{
        inputWeekday = GetSelectedDate(inputMonth);

        if(inputWeekday == -1){
        system("cls");
        AccessWeekdaysSchedules();
        FinalPrint();
    	}
    
    	else if(inputWeekday == 0){
    	ExecuteProgram();
		}

        else if(isSunday(currentYear, inputMonth, inputWeekday)){
            printf("\t\tYou entered a date that falls on a Sunday. Please try again.\n\n");
        }
    }while(isSunday(currentYear, inputMonth, inputWeekday));

    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    if(!isMonday(currentYear, inputMonth, inputWeekday)){
        printf("\tPlease select a time slot for the Service Scheduling\n");
        printf("\t\t[1] 06:00 AM \t\t[2] 05:15 PM\n\n");
        printf("\t\t[0] Go back to Main Menu\n");
    	printf("\t\t[-1] Go back to Access Sunday Schedules Menu\n\n");

        // This do-while loop will ask the user to select a time slot for a specific Sunday and check if it is a valid input.
        do{
            printf("\tSelected Time Slot: ");
            scanf(" %d", &inputTimeSlot);
            ClearInputBuffer();

            if(inputTimeSlot < -1 || inputTimeSlot > 2){
                printf("\t\t\tYou provided an invalid input. Please try again.\n");
            }		

            else if(inputTimeSlot == -1){
            system("cls");
            AccessSundaysSchedules();
            FinalPrint();
        	}
        
        	else if(inputMonth == 0){
        	ExecuteProgram;
    		}
    		
        }while(inputTimeSlot < -1 || inputTimeSlot > 2);
    }

    else{        
        inputTimeSlot = 1;
        printf("\tMondays only have one Mass schedule. The time slot is automatically recorded at ");
        PrintWeekdayTimeSlot(inputTimeSlot);
        printf("\n");
    }
    
    printf("\n-----------------------------------------------------------------------------------------------\n\n");

    // This line will call the function that would print the index of the existing members alongside their nicknames.
    PrintMembersWithNumberAndNickname();

    printf("\n-----------------------------------------------------------------------------------------------\n");

    // This code block (up until the end of this function) will check for the existing schedules in the SundaySchedules dynamic array.
    // If there is an existing schedule, then it will be open for modifications.
    // If there are no existing schedules, then it will be created and assigned accordingly. 
    scheduleEntryMatch = 0;

    for(idxCtr01 = 0; idxCtr01 < weekdayScheduleCount; idxCtr01++){
        // This value assignment will check for an exact match of an existing schedule in the SundaySchedules dynamic array.
        scheduleEntryMatch = WeekdaySchedules[idxCtr01].Year == currentYear &&
                             WeekdaySchedules[idxCtr01].Month == inputMonth &&
                             WeekdaySchedules[idxCtr01].Date == inputWeekday &&
                             WeekdaySchedules[idxCtr01].MassNumber == inputTimeSlot;

        // This if condition checks if there is both an existing schedule in the record and a schedule assignment for the said schedule.
        // This code block shall cater to both assigned and marked as 'NONE' positions, provided that not all date entries
        // would exist prior to running this program.
        if(scheduleEntryMatch){
            // This code block will print the  date and time slot of the existing schedule
            printf("\n\tCurrent Servers for ");
            PrintMonth(inputMonth);
            printf(" %d, %d at [", inputWeekday, currentYear);
            PrintWeekdayTimeSlot(inputTimeSlot);
            printf("]\n");

            // This code block will print the names of those assigned in a specific service schedule position.
            printf("\t\tFirst Reader: %s\n", WeekdaySchedules[idxCtr01].FirstReader);
            printf("\t\tSecond Reader: %s\n", WeekdaySchedules[idxCtr01].SecondReader);
            printf("\t\tPrayers of the Faithful (POF): %s\n", WeekdaySchedules[idxCtr01].POF);
            printf("\t\tCommentator: %s\n\n", WeekdaySchedules[idxCtr01].Commentator);
            printf("-----------------------------------------------------------------------------------------------\n");

            // This code line will call the function that allows the user to redesignate the schedules of the servers.
            RedesignateServers(WeekdaySchedules, idxCtr01);
            break;
        }
    }

    // Should the schedule date not yet exist in the SundaySchedules dynamic array, then this condition will be performed.
    if(!scheduleEntryMatch){
        // This line increments the number of schedules stored in the SundaySchedules dynamic array.
        weekdayScheduleCount++; 
        
        // This line will reallocate memory to cater the new entry to be encoded to the SundaySchedules dynamic array.
	    WeekdaySchedules = (HolyMass *)realloc(WeekdaySchedules, sizeof(HolyMass) * weekdayScheduleCount);

        // This code line will call the function that allows the user to add new server schedules into the SundaySchedules dynamic array.
        DesignateNewServers(WeekdaySchedules, weekdayScheduleCount - 1, currentYear, inputMonth, inputWeekday, inputTimeSlot, &serverOne, &serverTwo, &serverThree, &serverFour);
    }   
}

// Viewing Schedule Functions
void ViewSchedule(int isASunday){
    HolyMass *tempDatabase = NULL;
    int tempScheduleCount;

    PrintAppHeader();

    if(isASunday){
        printf("\tEXISTING SUNDAY SCHEDULES\n\n");            
        qsort(SundaySchedules, sundayScheduleCount, sizeof(HolyMass), compareSundaySchedules);
        
        tempScheduleCount = sundayScheduleCount;
        tempDatabase = (HolyMass *)realloc(tempDatabase, tempScheduleCount * sizeof(HolyMass));
        tempDatabase = SundaySchedules;        
    }

    else{
        printf("\tEXISTING WEEKDAY SCHEDULES\n\n");        
        qsort(WeekdaySchedules, weekdayScheduleCount, sizeof(HolyMass), compareSundaySchedules);
        
        tempScheduleCount = weekdayScheduleCount;
        tempDatabase = (HolyMass *)realloc(tempDatabase, tempScheduleCount * sizeof(HolyMass));
        tempDatabase = WeekdaySchedules;        
    }

    for(idxCtr01 = 0; idxCtr01 < tempScheduleCount; idxCtr01++){
        printf("\t");
        PrintMonth(tempDatabase[idxCtr01].Month);
        printf(" %d, %d ", tempDatabase[idxCtr01].Date, tempDatabase[idxCtr01].Year);
        printf("at [");
            if(isSunday(tempDatabase[idxCtr01].Year, tempDatabase[idxCtr01].Month, tempDatabase[idxCtr01].Date))
                PrintSundayTimeSlot(tempDatabase[idxCtr01].MassNumber);
            else    PrintWeekdayTimeSlot(tempDatabase[idxCtr01].MassNumber);
        printf("]\n");
        printf("\t\tFirst Reader: %s\n", tempDatabase[idxCtr01].FirstReader);
        printf("\t\tSecond Reader: %s\n", tempDatabase[idxCtr01].SecondReader);
        printf("\t\tPrayers of the Faithful: %s\n", tempDatabase[idxCtr01].POF);
        printf("\t\tCommentator: %s\n\n", tempDatabase[idxCtr01].Commentator);
    }
}

// File Handling Functions
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

    MinistryMembers = (MinistryMember *)realloc(MinistryMembers, (idxCtr01 + 1) * sizeof(MinistryMember));
	
    if(MinistryMembersDatabase == NULL)  return;
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
            MinistryMembers = (MinistryMember *)realloc(MinistryMembers, (idxCtr01 + 1) * sizeof(MinistryMember));	        	
    	}

    	ministryMemberCount = idxCtr01;
    	fclose(MinistryMembersDatabase);
	}   
}

void ExportSchedules(HolyMass *scheduleDatabase, int *scheduleCount, int isSunday){
    FILE *file;
    char *filename;

    if (isSunday) filename = "SundayScheduleDatabase.txt";
    else filename = "WeekdayScheduleDatabase.txt";

    file = fopen(filename, "w");

    for (idxCtr01 = 0; idxCtr01 < *scheduleCount; idxCtr01++) {
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].Year);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].Month);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].Date);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].MassNumber);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].isScheduled[0]);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].isScheduled[1]);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].isScheduled[2]);
        fprintf(file, "%d, ", scheduleDatabase[idxCtr01].isScheduled[3]);
        fprintf(file, "%s, ", scheduleDatabase[idxCtr01].FirstReader);
        fprintf(file, "%s, ", scheduleDatabase[idxCtr01].SecondReader);
        fprintf(file, "%s, ", scheduleDatabase[idxCtr01].POF);
        fprintf(file, "%s\n", scheduleDatabase[idxCtr01].Commentator);
    }

    fclose(file);
}

void ImportSchedules(HolyMass **scheduleDatabase, int *scheduleCount, char *filename){
    int idxCtr01 = 0;
    FILE *file = fopen(filename, "r");

    if (file == NULL) return;

    *scheduleDatabase = (HolyMass *)realloc(*scheduleDatabase, (idxCtr01 + 1) * sizeof(HolyMass));

    while (fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d, %[^,], %[^,], %[^,], %[^\n]",
                  &(*scheduleDatabase)[idxCtr01].Year,
                  &(*scheduleDatabase)[idxCtr01].Month,
                  &(*scheduleDatabase)[idxCtr01].Date,
                  &(*scheduleDatabase)[idxCtr01].MassNumber,
                  &(*scheduleDatabase)[idxCtr01].isScheduled[0],
                  &(*scheduleDatabase)[idxCtr01].isScheduled[1],
                  &(*scheduleDatabase)[idxCtr01].isScheduled[2],
                  &(*scheduleDatabase)[idxCtr01].isScheduled[3],
                  (*scheduleDatabase)[idxCtr01].FirstReader,
                  (*scheduleDatabase)[idxCtr01].SecondReader,
                  (*scheduleDatabase)[idxCtr01].POF,
                  (*scheduleDatabase)[idxCtr01].Commentator) != EOF) {
        idxCtr01++;
        (*scheduleDatabase) = (HolyMass *)realloc((*scheduleDatabase), (idxCtr01 + 1) * sizeof(HolyMass));
    }

    *scheduleCount = idxCtr01;
    fclose(file);
}

// Miscellaneous Functions
void ClearInputBuffer(){
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

void FinalPrint(){
printf("\tDo you wish to perform other actions in this program?.\n");
    printf("\t\t[Y] or [y] for YES\n"); 
    printf("\t\tAny character for NO\n"); 
    printf("\n\t\tUser Input: ");
    scanf(" %c", &toContinue);
    PrintAppHeader();
    ClearInputBuffer();	
    printf("==========================================================================================================\n");

    // If the user wants to continue, clear the screen and execute the program again.
    if(toContinue == 'Y' || toContinue == 'y'){ system("cls"); ExecuteProgram(); }
    else ExitProgram(0);
}

void ExitProgram(int errorCode){
	BubbleSort();
    ExportMinistryMembers();
    ExportSchedules(SundaySchedules, &sundayScheduleCount, 1);
    ExportSchedules(WeekdaySchedules, &weekdayScheduleCount, 0);

    free(MinistryMembers);
    free(SundaySchedules);
    free(WeekdaySchedules);

	exit(0);
}
