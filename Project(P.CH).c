#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define WHITE_BACKGROUND_COLOR "\033[1;47m"
#define GREEN_TEXT "\033[1;32m"
#define RED_TEXT "\033[1;31m"
#define RESET "\033[0m"
#define WHITE_BACKGROUND_GREEN_TEXT "\033[1;32;47m"
#define WHITE_BACKGROUND_YELLOW_TEXT "\033[1;33;47m"
#define WHITE_BACKGROUND_ORANGE_TEXT "\033[1;38;5;208;47m"
#define WHITE_BACKGROUND_BLUE_TEXT "\033[1;34;47m"

void clearScreen()
{
    system("cls");
}

int isLeapYear(int year)
{
    if ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int determineDaycode(int year, int month)
{

    int daysInMonths[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
    int daycode = (year * 365 + (year / 4) + 1) % 7;

    if (year >= 1206 && year <= 1218)
    {
        daycode += 2;
    }

    if (year == 1219)
    {
        daycode = (daycode + 2) % 7;
    }

    if (year >= 1220 && year < 1300)
    {
        daycode = (daycode + 1) % 7;
    }

    for (int i = 1; i < month; i++)
    {
        daycode = (daycode + daysInMonths[i]) % 7;
    }

    return daycode;
}

struct tm getCurrentDateAndTime() {
    time_t t = time(NULL);
    struct tm current_date = *localtime(&t);
    return current_date;
}

void miladiToShamsi(int year, int month, int day, int *sYear, int *sMonth, int *sDay) {

    int countDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, dayYear;
    int newMonth, newYear, newDay;

    dayYear = 0;

    for (i = 1; i < month; i++) {
        dayYear = dayYear + countDays[i - 1];
    }

    dayYear = dayYear + day;

    if (isLeapYear(year) && month > 2){
        dayYear = dayYear + 1;
    }

    if (dayYear <= 79) {
        if ((year - 1) % 4 == 0) {
            dayYear = dayYear + 11;
        } 
        else {
            dayYear = dayYear + 10;
        }

        newYear = year - 622;

        if (dayYear % 30 == 0) {
            newMonth = (dayYear / 30) + 9;
            newDay = 30;
        } 
        else {
            newMonth = (dayYear / 30) + 10;
            newDay = dayYear % 30;
        }
    } 
    else {
        newYear = year - 621;
        dayYear = dayYear - 79;

        if (dayYear <= 186) {
            if (dayYear % 31 == 0) {
                newMonth = dayYear / 31;
                newDay = 31;
            } 
            else {
                newMonth = (dayYear / 31) + 1;
                newDay = dayYear % 31;
            }
        } 
        else {
            dayYear = dayYear - 186;

            if (dayYear % 30 == 0) {
                newMonth = (dayYear / 30) + 7;
                newDay = 30;
            } 
            else {
                newMonth = (dayYear / 30) + 8;
                newDay = dayYear % 30;
            }
        }
    }

    *sYear = newYear;
    *sMonth = newMonth;
    *sDay = newDay;
}

void shamsiToMiladi(int y, int m, int d, int *gYear, int *gMonth, int *gDay) {
   
    int sumShamsi[] = {31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365};
    int miladiDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int yy, mm, dd, dayCount;
    dayCount = d;
    
    if (m > 1)
        dayCount = dayCount + sumShamsi[m - 2];
    yy = y + 621;
    dayCount = dayCount + 79;

    if (isLeapYear(yy) == 1) {
        if (dayCount > 366) {
            dayCount = dayCount - 366;
            yy = yy + 1;
        }
    } 
    else if (dayCount > 365) {
        dayCount = dayCount - 365;
        yy = yy + 1;
    }

    if (isLeapYear(yy) == 1)
        miladiDays[1] = 29;

    mm = 0;
    while (dayCount > miladiDays[mm]) {
        dayCount = dayCount - miladiDays[mm];
        mm = mm + 1;
    }

    
    *gYear = yy;
    *gMonth = mm + 1;
    *gDay = dayCount;

}

int intPart(double value) {
    if (value < 0) {
        return (int)(value - 0.5);
    } 
    else {
        return (int)(value + 0.5);
    }
}

void miladiToLunar(int year, int month, int day, int *lYear, int *lMonth, int *lDay)
{
    int juliandate;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
    {
        juliandate = intPart((1461 * (year + 4800 + intPart((month - 14) / 12))) / 4)
                     + intPart((367 * (month - 2 - 12 * intPart((month - 14) / 12))) / 12)
                     - intPart((3 * intPart((year + 4900 + intPart((month - 14) / 12)) / 100)) / 4)
                     + day - 32075;
    }
    else
    {
        juliandate = 367 * year
                     - intPart((7 * (year + 5001 + intPart((month - 9) / 7))) / 4)
                     + intPart((275 * month) / 9)
                     + day + 1729777;
    }

    int l = juliandate - 1948440 + 10632;
    int n = intPart((l - 1) / 10631);
    l = l - 10631 * n + 354;

    int j = (intPart((10985 - l) / 5316)) * (intPart((50 * l) / 17719))
            + (intPart(l / 5670)) * (intPart((43 * l) / 15238));

    l = l - (intPart((30 - j) / 15)) * (intPart((17719 * j) / 50))
        - (intPart(j / 16)) * (intPart((15238 * j) / 43)) + 29;

    int lunarM = intPart((24 * l) / 709);
    int lunarD = l - intPart((709 * lunarM) / 24);
    int lunarY = 30 * n + j - 30;

    *lYear = lunarY; 
    *lMonth = lunarM; 
    *lDay = lunarD; 
}

void calculateAge()
{
    int birthYear, birthMonth, birthDay;

        do{
            printf("[0] Back to menu\n");
            printf("----------------------------\n");
            printf("Enter your birth year(1206-1498):");
            scanf("%d", &birthYear);
        
            if(birthYear == 0){
                clearScreen();
                printf("[0] Back to menu!\n");
                printf("----------------------------\n");
                main();
            }
            else if(birthYear < 1206 || birthYear > 1498){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    calculateAge();
            }

            printf("Enter your birth month(1-12):");
            scanf("%d", &birthMonth);

            if(birthMonth == 0){
                clearScreen();
                printf("[0] Back to menu!\n");
                printf("----------------------------\n");
                main();
            }
            else if(birthMonth < 1 || birthMonth > 12){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    calculateAge();
            }

            printf("Enter your birth day(1-31):");
            scanf("%d", &birthDay);
        
            if(birthDay == 0){
                clearScreen();
                printf("[0] Back to menu!\n");
                printf("----------------------------\n");
                main();
            } 
            else if(birthDay < 1 || birthDay > 31){
                printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                printf("----------------------------\n");
                calculateAge();
            }

            clearScreen();
            printf("----------------------------\n");

    int daysInPersianMonths[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
    struct tm current_date = getCurrentDateAndTime();

    int current_miladi_year = current_date.tm_year + 1900;
    int current_miladi_month = current_date.tm_mon;
    int current_miladi_day = current_date.tm_mday;

    int current_year, current_month, current_day;
    miladiToShamsi(current_miladi_year, current_miladi_month, current_miladi_day, &current_year, &current_month, &current_day);

    int age_years = current_year - birthYear;
    int age_months = current_month - birthMonth;
    int age_days = current_day - birthDay;

    if (age_days < 0)
    {
        age_months--;
        age_days += daysInPersianMonths[current_month];
    }

    if (age_months < 0)
    {
        age_years--;
        age_months += 12;
    }

    int birthdate_daycode = determineDaycode(birthYear, birthMonth);
    int day_of_week_index = (birthdate_daycode + birthDay - 1) % 7;

    printf("\n Your Age: %d years, %d months, %d days", age_years, age_months, age_days);

    if (age_years < 0)
    {
        clearScreen();
        printf("\n%sInvalid input!%s\n\n", RED_TEXT, RESET);
        return;
    }

    char *days_of_week_persian[] = {"SHANBE", "YEKSHANBE", "DOSHANBE", "SESHANBE", "CHAHARSHANBE", "PANJESHANBE", "JOOMEH"};
    char *days_of_week_miladi[] = {"SATURDAY", "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY"};
    printf("\n You were born on %s\n",days_of_week_persian[day_of_week_index]);

    int gYear, gMonth, gDay;
    shamsiToMiladi(birthYear, birthMonth, birthDay, &gYear, &gMonth, &gDay);

    birthYear = gYear;
    birthMonth = gMonth;
    birthDay = gDay;

    time_t birth_time = mktime(&(struct tm){0, 0, 0, birthDay, birthMonth - 1, birthYear - 1900});
    time_t current_time = mktime(&current_date);

    int seconds_lived = difftime(current_time, birth_time);
    int days_lived = seconds_lived / (60 * 60 * 24);

    printf("\n Number of days passed since your birth: %d\n", days_lived);

    printf("\n Miladi birth date: %d/%02d/%02d", gYear, gMonth, gDay);
    printf(" %s\n", days_of_week_miladi[day_of_week_index]);
    printf("----------------------------\n");
    }while(birthYear != 0 || birthMonth != 0 || birthDay != 0);
}

void Calendar()
{

    int year, month;

        do{
            printf("[0] Back to menu\n");
            printf("----------------------------\n");
            printf("Enter year(1206-1498):");
            scanf("%d", &year);

            if(year == 0){
                clearScreen();
                printf("[0] Back to menu!\n");
                printf("----------------------------\n");
                main();
            }
            else if(year < 1206 || year > 1498){
                clearScreen();
                printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                printf("----------------------------\n");
                Calendar();
            }

            printf("Enter month(1-12):");
            scanf("%d", &month);

            if(month == 0){
                clearScreen();
                printf("[0] Back to menu!\n");
                printf("----------------------------\n");
                main();
            }
            else if(month < 1 || month > 12){
                clearScreen();
                printf("----------------------------\n");
                printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                printf("----------------------------\n");
                Calendar();
            }

            clearScreen();
            int daysInMonths[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
  
            if (isLeapYear(year) == 1)
            {
                int daysInMonths[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30};
            }

            switch(month){
                case 1:
                    printf("%s          Farvardin         %s\n", WHITE_BACKGROUND_GREEN_TEXT, RESET);
                    break;
                case 2:
                    printf("%s         Ordibehesht        %s\n", WHITE_BACKGROUND_GREEN_TEXT, RESET);
                    break;
                case 3:
                    printf("%s           Khordad          %s\n", WHITE_BACKGROUND_GREEN_TEXT, RESET);
                    break;
                case 4:
                    printf("%s             Tir            %s\n", WHITE_BACKGROUND_YELLOW_TEXT, RESET);
                    break;
                case 5:
                    printf("%s            Mordad          %s\n", WHITE_BACKGROUND_YELLOW_TEXT, RESET);
                    break;
                case 6:
                    printf("%s          Shahrivar         %s\n", WHITE_BACKGROUND_YELLOW_TEXT, RESET);
                    break;
                case 7:
                    printf("%s             Mehr           %s\n", WHITE_BACKGROUND_ORANGE_TEXT, RESET);
                    break;
                case 8:
                    printf("%s             Aban           %s\n", WHITE_BACKGROUND_ORANGE_TEXT, RESET);
                    break;
                case 9:
                    printf("%s             Azar           %s\n", WHITE_BACKGROUND_ORANGE_TEXT, RESET);
                    break;
                case 10:
                    printf("%s             Dey            %s\n", WHITE_BACKGROUND_BLUE_TEXT, RESET);
                    break;
                case 11:
                    printf("%s            Bahman          %s\n", WHITE_BACKGROUND_BLUE_TEXT, RESET);
                    break;
                case 12:
                    printf("%s            Esfand          %s\n", WHITE_BACKGROUND_BLUE_TEXT, RESET);
                    break;
            }
            int dayCode = determineDaycode(year, month);

            printf("%s  SH  YE  DO  SE  CH  PA%s  %sJO%s\n", GREEN_TEXT, RESET, RED_TEXT, RESET);


            for(int day = 1; day <= dayCode % 7; day++){
                printf("    ");
            }

            for(int day = 1; day <= daysInMonths[month]; day++){

                if((day + dayCode)% 7 == 1){
                    printf("\n");
                }   

                printf("%4d", day);
            }

            printf("\n----------------------------\n");
        }while(year != 0 || month != 0);
        
        
}

int main()
{
    
    int input;

    do
    {
        printf("[0] Quit\n");
        printf("[1] Calendar\n");
        printf("[2] Age\n");
        printf("[3] Conversion\n");
        printf("----------------------------\n");
        printf("select option:");
        scanf("%d", &input);

        if(input == 0){
            clearScreen();
            printf("bye!");
            break;
        }
        else if(input == 1){
            clearScreen();
            Calendar(); 
        }
        else if(input == 2){
            clearScreen();
            calculateAge();
        }
        else if(input == 3){
            int input2;
            clearScreen();
            printf("[0] Back to menu\n");
            printf("----------------------------\n");
            printf("[1] Shamsi to Others\n");
            printf("[2] Miladi to Others\n");
            printf("----------------------------\n");
            printf("select option:");
            scanf("%d", &input2);

            if(input2 == 1){
                clearScreen();
                int sYear, sMonth, sDay;
                int gYear, gMonth, gDay;
                int lYear, lMonth, lDay;
            do{
                printf("----------------------------\n");    
                printf("[0] Back to menu\n");
                printf("----------------------------\n");
                printf("Enter year in Shamsi format(1206-1498):");
                scanf("%d", &sYear);

                if(sYear == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }
                else if(sYear < 1206 || sYear > 1498){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    main();
                }

                printf("Enter month in Shamsi format(1-12):");
                scanf("%d", &sMonth);

                if(sMonth == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }
                else if(sMonth < 1 || sMonth > 12){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    main();
                }

                printf("Enter day in Shamsi format(1-31):");
                scanf("%d", &sDay);

                if(sDay == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }
                else if(sDay < 1 || sDay > 31){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    main();
                }
                clearScreen();
                printf("----------------------------\n");

                shamsiToMiladi(sYear, sMonth, sDay, &gYear, &gMonth, &gDay);
                miladiToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);
                printf("Miladi: %d/%d/%d\n",gYear, gMonth, gDay);
                printf("Ghamari: %d/%d/%d\n", lYear, lMonth, lDay);
        }while(sYear != 0 || sMonth != 0 || sDay != 0);
        }
        else if(input2 == 2){
            clearScreen();
            int sYear, sMonth, sDay;
            int gYear, gMonth, gDay;
            int lYear, lMonth, lDay;
            do{
                printf("----------------------------\n");    
                printf("[0] Back to menu\n");
                printf("----------------------------\n");
                printf("Enter year in Miladi format(1827-2120):");
                scanf("%d", &gYear);

                if(gYear == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }
                else if(gYear < 1827 || gYear > 2120){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    main();
                }

                printf("Enter month in Miladi format(1-12):");
                scanf("%d", &gMonth);

                if(gMonth == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }

                printf("Enter day in Miladi format:(1-31):");
                scanf("%d", &gDay);

                if(gDay == 0){
                    clearScreen();
                    printf("[0] Back to menu!\n");
                    printf("----------------------------\n");
                    main();
                }
                else if(gDay < 1 || gDay > 31){
                    clearScreen();
                    printf("%sInvalid input!%s\n", RED_TEXT, RESET);
                    printf("----------------------------\n");
                    main();
                }

                clearScreen();
                printf("----------------------------\n");

                miladiToShamsi(gYear, gMonth, gDay, &sYear, &sMonth, &sDay);
                miladiToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);
                printf("Shamsi: %d/%d/%d\n",sYear, sMonth, sDay);
                printf("Ghamari: %d/%d/%d\n", lYear, lMonth, lDay);
        }while(gYear != 0 || gMonth != 0 || gDay != 0);
        }
        else if(input2 == 0){
            clearScreen();
            printf("[0] Back to menu!\n");
            printf("----------------------------\n");
            main();
        }
    } 
    else {
        clearScreen();
        printf("%sInvalid input!%s\n", RED_TEXT, RESET);
        printf("----------------------------\n");
        main();
    }

    } while (input != 0);

}
