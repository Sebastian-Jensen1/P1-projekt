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
    char *beskrivelse;
} By;


int laes_fra_fil(const char *filnavn, By by_array[], int *antal_byer);
void laes_alle_byer(FILE *file, By by_array[], int *length);
void print_by(By by);
By *filtrer_oplevelse(By by_array[], int *antal_byer, int valg);
By *filtrer_radius(By by_array[], int *antal_byer, int valg_radius);
By *filtrer_pris(By by_array[], int *antal_byer, int prisklasse);
double beregn_co2_udledning(double afstand, int transportmiddel);
void filtrer_byer(By by_array[], int *antal_byer);
int laes_fra_fil1(const char *filnavn, By by_array[]);


int main(void){
    int antal_byer = 0;
    By by_array[ANTAL_BYER] = {0};  
    
    laes_fra_fil("data.csv", by_array, &antal_byer);

    laes_fra_fil1("test.csv", by_array);
    
    //fjernes i det endelige program
    printf("Antal byer indlæst: %d\n", antal_byer);

    //printer ny header. fjernes i det endelige program
    printf("\n%-10s %-25s %-15s %-15s %-15s %-15s %-10s %-13s %-13s\n", 
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
    int oplevelses_valg_2;
    int radius;
    int prisklasse;
    int transportmiddel;
    int transportmiddel_2;
    double co2;

    //radius
    printf("Vælg radius:\n1: 5-15 km\n2: 15-30 km\n3: 30-45 km\n4: 45-60 km\n5: 60-75 km\n");
    scanf(" %d", &radius);

    while (radius < 1 || radius > 8){
        printf("Vælg radius: 1: 5-15 km\n2: 15-30 km\n3: 30-45 km\n4: 45-60 km\n5: 60-75 km\n");
        scanf(" %d", &radius);
    }

    filtrer_radius(by_array, antal_byer, radius);
    //radius slut


    //vælg budget
    printf("Vælg prisklasse:\n1: budget\n2: mellemklasse\n3: luksus\n");
    scanf(" %d", &prisklasse);
    filtrer_pris(by_array, antal_byer, prisklasse);


    
    //vælger oplevelse og gem byer som opfylder kriteriet i filtrerede_byer
    printf("Hvilken type oplevelse ønsker du primært?\n1: Landskab\n2: Historisk\n3: Vand\n");
    scanf(" %d", &oplevelses_valg);
    
    printf("Hvilken type oplevelse ønsker du sekundært\n");
    scanf(" %d", &oplevelses_valg_2);
    
    while (oplevelses_valg < 1 || oplevelses_valg > 3){
        printf("Vælg venligst mellem 1, 2 og 3: [1=Landskab, 2=Historisk, 3=Vand]: ");
        scanf(" %d", &oplevelses_valg);
    }
    
    filtrer_oplevelse(by_array, antal_byer, oplevelses_valg); //&midl_antal_byer kommer fra *ny_antal_byer
    //oplevelse slut



    //printer vores recommendations
    for (int i = 0; i < *antal_byer; i++){
        printf("\nVi foreslår at du rejser til %s.\n", by_array[i].navn);
        printf("%s er %s\n \n",by_array[i].navn, by_array[i].beskrivelse);

    if (*antal_byer > 1){
        filtrer_oplevelse(by_array, antal_byer, oplevelses_valg_2);
    }
    
    //VI SKAL NOK FINDE DET BEDSTE MATCH OG PRINTE CO2 FOR DET KUN. så det ikke er en del af for_loopet

    //beregner co2_udledning
    

    printf("Hvordan ønsker du at rejse?\n1: fly\n2: bus\n3: elbil\n4: tog\n");
    scanf(" %d", &transportmiddel);

    //hvis man vælger fly skal man også fra lufthavnen til hidden gem
     if (transportmiddel == 1){
        co2 = beregn_co2_udledning(by_array[i].km_DK_Vluft, transportmiddel);
        printf("Hvordan ønsker du at komme fra Venedig lufthavn til %s?\n2: bus\n3: elbil\n4: tog\n", by_array[i].navn);
        scanf(" %d", &transportmiddel_2);
        co2 += beregn_co2_udledning(by_array[i].km_Vluft_by, transportmiddel_2);
    } else {
       co2 = beregn_co2_udledning(by_array[i].km_DK_by, transportmiddel);
    }
    // et træ absorberer ca 20 kg  C02 pr år. Det er omskrevet til gram, derfor 20.000.
    
    int antal_traer = co2 / 20000;
    if (antal_traer > 1){
    printf("Din rejse udleder omkring %0.2lf gram CO2 pr, passager.\n Der skal %d træer til for at absorbere det på et år.\n", co2, antal_traer);
    } else {
    printf("Din rejse udleder omkring %0.2lf gram CO2 pr, passager.\n Der skal %d træ til for at absorbere det på et år.\n", co2, antal_traer);
    } 
    }
    
    }

//funktion til filtrering af prisklasse
By *filtrer_pris(By by_array[], int *antal_byer, int prisklasse){
    int count = 0;

    for (int i = 0; i < *antal_byer; i++) {
        if ((prisklasse == 1 && strcmp(by_array[i].prisniveau, "Budget") == 0)       ||
           (prisklasse == 2 && strcmp(by_array[i].prisniveau, "Mellemklasse") == 0)  ||
           (prisklasse == 3 && strcmp(by_array[i].prisniveau, "Luksus") == 0)){
           by_array[count++] = by_array[i]; 
           }

    }
    *antal_byer = count;    //opdaterer antallet af filtrerede byer
    return by_array;        //returner det nye array

}

// Funktion til beregning af CO2-udledning
double beregn_co2_udledning(double afstand, int transportmiddel) {
    // CO2-udledning pr. km i gram
    const double CO2_FLY = 285.0;       // Fly
    const double CO2_BUS = 68.0;        // Bus
    const double CO2_ELBIL = 20.0;      // El-bil
    const double CO2_TOG = 14.0;        // Tog

    switch (transportmiddel) {
        case 1: // Fly
            return afstand * CO2_FLY;
        case 2: // Bus
            return afstand * CO2_BUS;
        case 3: // El-bil
            return afstand * CO2_ELBIL;
        case 4: // Tog
            return afstand * CO2_TOG;
        default:
            return -1; // Ugyldigt transportmiddel
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
    
    printf("%-10d %-25s %-15s %-15d %-15s %-15s %-10d %-13d %-13d\n", 
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
    while (fscanf(file, "%d,%49[^,],%19[^,],%d,%19[^,],%19[^,],%d,%d,%d\n", 
                   &by.index, by.navn, by.prisniveau, &by.km_til_Venedig, 
                   by.oplev_1, by.oplev_2, &by.km_DK_by, &by.km_DK_Vluft,
                   &by.km_Vluft_by) == 9) { 
        by_array[i] = by;
        i++;

        //stopper indlæsning, hvis vi når slutningen af filen eller overskrider arraystørrelsen
        if (i >= ANTAL_BYER) {
            break;
        }
    }
    *length = i;
}

//funktion til at indlæse  by-csvfil
int laes_fra_fil(const char *filnavn, By by_array[], int *antal_byer){
    FILE *file = fopen("data.csv", "r");
    if (file == NULL){
        exit(EXIT_FAILURE);
    } else {

        //indlæs alle byerne i et array af structs:
        laes_alle_byer(file, by_array, antal_byer);

        fclose(file);
    }
    return 1;
}

int laes_fra_fil1(const char *filnavn, By by_array[]) {
    FILE *file1 = fopen("test.csv", "r");
    if (file1 == NULL) {
        exit(EXIT_FAILURE);
    }

    char linje[512];
    int index;
    char beskrivelse[500];

    // Læs headeren
    fgets(linje, sizeof(linje), file1);

    // Læs hver linje og match index
    while (fgets(linje, sizeof(linje), file1)) {
        if (sscanf(linje, "%d, %[^\n]", &index, beskrivelse) == 2) {
            for (int i = 0; i < ANTAL_BYER; i++) {
                if (by_array[i].index == index) {
                    by_array[i].beskrivelse = strdup(beskrivelse); // Alloker og kopier beskrivelsen
                    break; // Stop, når vi har fundet den rigtige by
                }
            }
        }
    }

    fclose(file1);
    return 1;
}

