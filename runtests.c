#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANTAL_BYER 100

typedef struct {
    int index;
    char navn[50];
    char prisniveau[20];
    int km_til_Venedig;
    char oplev_1[20];
    char oplev_2[20];
    int km_DK_by;
    int km_DK_Vluft;
    int km_Vluft_by;
} By;



int laes_fra_fil(const char *filnavn, By by_array[], int *antal_byer);
void laes_alle_byer(FILE *file, By by_array[], int *length);
void print_by(By by);
By *filtrer_oplevelse(By by_array[], int *antal_byer, int valg);
By *filtrer_radius(By by_array[], int *antal_byer, int valg_radius);
void filtrer_byer(By by_array[], int *antal_byer);



int main(void){
    int antal_byer = 0;
    By by_array[ANTAL_BYER] = {0};  
    
    laes_fra_fil("data.csv", by_array, &antal_byer);
    
    //fjernes i det endelige program
    printf("Antal byer indlæst: %d\n", antal_byer);

    //printer ny header. fjernes i det endelige program
    printf("\n%-10s %-15s %-15s %-15s %-15s %-15s %-15s %-15s %-15s\n", 
           "Index", "By", "Prisniveau", "km_til_Venedig", "Oplev_1", "Oplev_2", 
           "km_DK_by", "km_DK_Vluft", "km_Vluft_by");
    
    //printer de indlæste byer. fjernes i det endelige program
    for (int i = 0; i < antal_byer; i++){
        print_by(by_array[i]);
    }


    //udskriver vores forslag
    filtrer_byer(by_array, &antal_byer);
   


//lave qsort på oplev1 og oplev2??????????


    return 0;
}

//her kalder vi alle vores funktioner og printer vores recommendation
//*antal_byer er en pointer fra de funktioner vi kalder, fordi antallet af byer ændrer sig undervejs
void filtrer_byer(By by_array[], int *antal_byer){
    int midl_antal_byer = 0;
    int oplevelses_valg;
    int radius;

    //radius
    printf("Vaelg radius: 1=5-15 km, 2=15-30 km, 3=30-45 km, 4=45-60 km, 5=60-75 km\n");
    scanf(" %d", &radius);

    while (radius < 1 || radius > 8){
        printf("Vaelg mellem: 1=5-15 km, 2=15-30 km, 3=30-45 km, 4=45-60 km, 5=60-75 km\n");
        scanf(" %d", &radius);
    }

    filtrer_radius(by_array, antal_byer, radius);
    //radius slut




    
    //vælger oplevelse og gem byer som opfylder kriteriet i filtrerede_byer
    printf("Hvilken type oplevelse ønsker du? [1=Landskab, 2=Historisk, 3=Vand]: ");
    scanf(" %d", &oplevelses_valg);
    
    while (oplevelses_valg < 1 || oplevelses_valg > 3){
        printf("Vaelg mellem 1, 2 og 3: [1=Landskab, 2=Historisk, 3=Vand]: ");
        scanf(" %d", &oplevelses_valg);
    }
    
    filtrer_oplevelse(by_array, antal_byer, oplevelses_valg); //&midl_antal_byer kommer fra *ny_antal_byer
    //oplevelse slut




    //printer vores recommendation
    for (int i = 0; i < *antal_byer; i++){
        printf("By: %s\n", by_array[i].navn);
    }
}

//funktion til at opdatere by_array med byer der opfylder km_til_venedig og fjerne de andre
By *filtrer_radius(By by_array[], int *antal_byer, int valg_radius){
    int count = 0;

    for (int i = 0; i < *antal_byer; i++) {
        if ((valg_radius == 1 && by_array[i].km_til_Venedig <= 15 && by_array[i].km_til_Venedig >= 5) ||
            (valg_radius == 2 && by_array[i].km_til_Venedig <= 30 && by_array[i].km_til_Venedig > 15) ||
            (valg_radius == 3 && by_array[i].km_til_Venedig <= 45 && by_array[i].km_til_Venedig > 30) ||
            (valg_radius == 4 && by_array[i].km_til_Venedig <= 60 && by_array[i].km_til_Venedig > 45) ||
            (valg_radius == 5 && by_array[i].km_til_Venedig <= 75 && by_array[i].km_til_Venedig > 60) ||
            (valg_radius == 6 && by_array[i].km_til_Venedig <= 90 && by_array[i].km_til_Venedig > 75) ||
            (valg_radius == 7 && by_array[i].km_til_Venedig <= 105 && by_array[i].km_til_Venedig > 90)||
            (valg_radius == 8 && by_array[i].km_til_Venedig <= 120 && by_array[i].km_til_Venedig > 105)){
            by_array[count++] = by_array[i]; 
        }
    }

    *antal_byer = count;    //opdaterer antallet af filtrerede byer
    return by_array;        //returner det nye array
}

//funktion til at opdatere by_array med byer der opfylder oplev_1 og fjerne de andre
By *filtrer_oplevelse(By by_array[], int *antal_byer, int valg_opl){
    int count = 0;

    for (int i = 0; i < *antal_byer; i++) {
        if ((valg_opl == 1 && strcmp(by_array[i].oplev_1, "Landskab") == 0) ||
            (valg_opl == 2 && strcmp(by_array[i].oplev_1, "Historisk") == 0) ||
            (valg_opl == 3 && strcmp(by_array[i].oplev_1, "Vand") == 0)) {
            by_array[count++] = by_array[i]; // Flyt byen frem i arrayet, hvis den opfylder kriteriet
        }
    }

    *antal_byer = count;    //opdaterer antallet af filtrerede byer
    return by_array;        //returner det nye array
}

//funktion til at printe en by
void print_by(By by){
    
    printf("%-10d %-15s %-15s %-15d %-15s %-15s %-15d %-15d %-15d\n", 
         by.index, by.navn, by.prisniveau, by.km_til_Venedig, 
           by.oplev_1, by.oplev_2, by.km_DK_by, by.km_DK_Vluft,
           by.km_Vluft_by);
}

//funktion til at lægge alle byer over i by_array
void laes_alle_byer(FILE *file, By by_array[], int *length){
    By by;
    int i = 0;

    //læser headeren først
    char header[256];
    fgets(header, sizeof(header), file);

    //indlæser filen i by_array
    while (fscanf(file, "%d,%49[^,],%19[^,],%d,%49[^,],%49[^,],%d,%d,%d\n", 
                   &by.index, by.navn, by.prisniveau, &by.km_til_Venedig, 
                   by.oplev_1, by.oplev_2, &by.km_DK_by, &by.km_DK_Vluft,
                   &by.km_Vluft_by) == 9) { // Tjek for 9 succesfuldt læste værdier
        by_array[i] = by;
        i++;

        //stopper indlæsning, hvis vi når slutningen af filen eller overskrider arraystørrelsen
        if (i >= ANTAL_BYER) {
            break;
        }
    }
    *length = i;
}

//funktion til at indlæse csvfil
int laes_fra_fil(const char *filnavn, By by_array[], int *antal_byer){
    FILE *file = fopen("data.csv", "r");
    if (file == NULL){
    printf("Filen kan ikke aabnes");
    exit(EXIT_FAILURE);
    
    }
        //indlæs alle byerne i et array af structs:
        laes_alle_byer(file, by_array, antal_byer);

        fclose(file);
    
    return 1;
}
