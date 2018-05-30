#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

vector<string> HAINE_POSIBILE {"Pantaloni", "Rochie", "Camasa", "Palton", "Geaca", "Costum"};
vector<string> MASINI_POSIBILE {"Spalat", "Stors", "Uscat", "Calcat"};
int N_HAINE = 0;
int N_MASINI = 0;
const bool MASINILE_PORNESC_NUMAI_CAND_SUNT_PE_JUMATATE_PLINE = 0;

class haina{
public:
    /// ==================
    /// === Properties ===
    /// ==================

    /// Constructor
    string nume;
    double greutate;
    bool culoare_inchisa;

    /// Others
    bool necesita_spalare_peste_temperatura_minima = false;
    double temperatura_minima;

    bool necesita_spalare_sub_temperatura_maxima = false;
    double temperatura_maxima;

    bool haina_usoara = false;
    bool haina_grea = false;

    bool haina_se_curata_acum = false;
    bool este_spalata = false;
    bool este_stoarsa = false;
    bool este_uscata = false;
    bool este_calcata = false;

    /// ===============
    /// === Methods ===
    /// ===============

    haina(string Nume, double Greutate, bool Culoare_inchisa){
        if(find(begin(HAINE_POSIBILE), end(HAINE_POSIBILE), Nume) == HAINE_POSIBILE.end()){
            string msg = "Tipul de haina \"" + Nume + "\" nu exista!";
            throw runtime_error(msg);
        }

        ++N_HAINE;
        nume = Nume;
        if(nume == "Pantaloni" || nume == "Rochie" || nume == "Camasa")
            haina_usoara = true;
        if(nume == "Palton" || nume == "Geaca")
        {
            haina_grea = true;
            este_stoarsa = true;
        }

        greutate = Greutate;
        culoare_inchisa = Culoare_inchisa;
    }

    ~haina(){
    }

    void adauga_temperatura_minima(double temperatura){
        necesita_spalare_peste_temperatura_minima = true;
        temperatura_minima = temperatura;
    }

    void adauga_temperatura_maxima(double temperatura){
        necesita_spalare_sub_temperatura_maxima = true;
        temperatura_maxima = temperatura;
    }

    void status(){
        cout<<"--------------------\n";
        cout<<"Tipul hainei: "<<nume<<"\n";
        cout<<"Spalata: "<<este_spalata<<"\n";
        cout<<"Stoarsa: "<<este_stoarsa<<"\n";
        cout<<"Uscata:  "<<este_uscata<<"\n";
        cout<<"Calcata: "<<este_calcata<<"\n";
        cout<<"\n";
    }
};

class masina{
public:
    /// ==================
    /// === Properties ===
    /// ==================

    /// Constructor
    string tip_masina;
    double capacitate;
    double durata_functionare;

    /// Others
    bool masina_cu_echipament_special = false;
    bool are_temperatura_setata = false;
    double temperatura;
    vector<haina*> haine_in_masina;

    int timp_start_functionare = -1;
    int timp_sfarsit_functionare = -1;
    bool este_pornita = false;

    /// ===============
    /// === Methods ===
    /// ===============
    masina(string Tip_masina, double Capacitate, double Durata){
        if(find(begin(MASINI_POSIBILE), end(MASINI_POSIBILE), Tip_masina) == MASINI_POSIBILE.end()){
            string msg = "Tipul de masina \"" + Tip_masina + "\" nu exista!";
            throw runtime_error(msg);
        }

        ++N_MASINI;
        tip_masina = Tip_masina;
        capacitate = Capacitate;
        durata_functionare = Durata;
    }

    ~masina(){
    }

    void adauga_echipament_special(){
        masina_cu_echipament_special = true;
    }

    void seteaza_temperatura(double temp){
        are_temperatura_setata = true;
        temperatura = temp;
    }

    bool spala(haina H){
        /// Verifica daca poate spala
        if(tip_masina != "Spalat"){
            //cout<<"Aceasta masina nu poate spala!\n";
            return false;
        }
        if(H.haina_grea == true && masina_cu_echipament_special == false){
            //cout<<"Aceasta masina nu poate spala haina!\n";
            return false;
        }

        if(H.necesita_spalare_sub_temperatura_maxima || H.necesita_spalare_peste_temperatura_minima){
            if(are_temperatura_setata == false){
                //cout<<"Masina nu are temperatura setata! Haina nu poate fi spalata!\n";
                return false;
            }
            else{
                if(H.necesita_spalare_peste_temperatura_minima && temperatura < H.temperatura_minima){
                    //cout<<"Temperatura masinii este prea mica pentru aceasta haina!\n";
                    return false;
                }
                if(H.necesita_spalare_sub_temperatura_maxima && temperatura > H.temperatura_maxima){
                    //cout<<"Temperatura masinii este prea mare pentru aceasta haina!\n";
                    return false;
                }
            }
        }

        /// Spala
        const double CANTITATE_FIXA_DE_DETERGENT = 70;
        double detergent_utilizat;

        if(H.nume == "Camasa" || H.nume == "Rochie")
            detergent_utilizat = CANTITATE_FIXA_DE_DETERGENT;
        else if(H.nume == "Pantaloni" && H.culoare_inchisa == true)
            detergent_utilizat = CANTITATE_FIXA_DE_DETERGENT * 2;
        else
            detergent_utilizat = 100 * H.greutate;

        bool pot_adauga_in_masina = true;
        for(auto &ptr_h : haine_in_masina){
            haina h = *ptr_h;
            if(h.culoare_inchisa != H.culoare_inchisa){ /// Culorile hainelor difera (inchis / deschis)
                pot_adauga_in_masina = false;
            }
            bool am_costum = false;
            bool am_altceva_decat_costum_geaca_sau_palton = false;

            if(H.nume == "Costum" || h.nume == "Costum") am_costum = true;
            if(H.nume != "Costum" && H.nume != "Geaca" && H.nume != "Palton") am_altceva_decat_costum_geaca_sau_palton = true;
            if(h.nume != "Costum" && h.nume != "Geaca" && h.nume != "Palton") am_altceva_decat_costum_geaca_sau_palton = true;

            if(am_costum == true && am_altceva_decat_costum_geaca_sau_palton == true)
                pot_adauga_in_masina = false;
        }

        if(!pot_adauga_in_masina) return false;

        return true;
    }

    bool stoarce(haina H){
        if(tip_masina != "Stors"){
            //cout<<"Aceasta masina nu poate stoarce!\n";
            return false;
        }

        if(H.nume == "Palton" || H.nume == "Geaca"){
            //cout<<"Aceasta haina nu se stoarce!\n";
            return false;
        }

        return true;
    }

    bool usuca(haina H){
        if(tip_masina != "Uscat"){
            //cout<<"Aceasta masina nu poate usca!\n";
            return false;
        }

        return true;

    }

    bool calca(haina H){
        if(tip_masina != "Calcat"){
            //cout<<"Aceasta masina nu poate calca!\n";
            return false;
        }
        return true;
    }

    bool haina_are_loc_in_masina(haina H){
        if(tip_masina == "Spalat" || tip_masina == "Stors"){
            double greutate_curenta = 0;
            for(auto &ptr_h : haine_in_masina)
                greutate_curenta += ptr_h->greutate;

            return greutate_curenta + H.greutate <= capacitate;
        }
        else if(tip_masina == "Uscat"){
            return 1 + haine_in_masina.size() <= capacitate;
        }
        else if(tip_masina == "Calcat"){
            return haine_in_masina.size() == 0;
        }
        else throw runtime_error("Tipul masinii nu este recunoscut!");
    }

    bool masina_poate_efectua_operatia_acestui_tip_de_masina(string tip_masina, haina H){
        if(tip_masina == "Spalat") return spala(H);
        else if(tip_masina == "Stors") return stoarce(H);
        else if(tip_masina == "Uscat") return usuca(H);
        else if(tip_masina == "Calcat") return calca(H);
    }

    void adauga_haina(haina &H){
        haine_in_masina.push_back(&H);
        H.haina_se_curata_acum = true;
    }

    bool poate_fi_pornita(int timp){
        if(timp < timp_sfarsit_functionare) /// Masina inca lucreaza
            return false;

        if(haine_in_masina.size() == 0) /// Masina este goala
            return false;

        if(tip_masina == "Spalat" || tip_masina == "Stors"){
            double greutate_curenta = 0;
            for(auto &ptr_h : haine_in_masina)
                greutate_curenta += ptr_h->greutate;

            return greutate_curenta >= capacitate/2 * MASINILE_PORNESC_NUMAI_CAND_SUNT_PE_JUMATATE_PLINE;
        }
        else if(tip_masina == "Uscat"){
            return 1 + haine_in_masina.size() >= capacitate/2 * MASINILE_PORNESC_NUMAI_CAND_SUNT_PE_JUMATATE_PLINE;
        }
        else if(tip_masina == "Calcat"){
            return haine_in_masina.size() == 1;
        }
        else throw runtime_error("Tipul masinii nu este recunoscut!");
    }

    void porneste_masina(int timp){
        if(tip_masina == "Calcat"){
            haina h = *(haine_in_masina.front());
            //cout<<"_!@#%#$>>--->> CALC haina de tipul "<<h.nume<<"\n";

            if(h.nume == "Camasa") durata_functionare = 120 * h.greutate;
            else if(h.nume == "Pantaloni") durata_functionare = 90 * h.greutate;
            else if(h.nume == "Costum"){
                double GREUTATE_SACOU = 1;
                durata_functionare = 120 * GREUTATE_SACOU + 90 * h.greutate;
            }
            else durata_functionare = 50;
        }

        timp_start_functionare = timp;
        timp_sfarsit_functionare = timp + durata_functionare;
        este_pornita = true;
    }

    void opreste_masina(){
        este_pornita = false;
    }

    void scoate_hainele_din_masina(){
        for(auto &ptr_h : haine_in_masina){
            if(tip_masina == "Spalat") ptr_h->este_spalata = true;
            else if(tip_masina == "Stors") ptr_h->este_stoarsa = true;
            else if(tip_masina == "Uscat") ptr_h->este_uscata = true;
            else if(tip_masina == "Calcat") ptr_h->este_calcata = true;
            else throw runtime_error("Tipul masinii nu este recunoscut!");
            ptr_h->haina_se_curata_acum = false;
            cout<<"Haina "<<ptr_h->nume<<" a fost spalata / stoarsa / uscata / calcata\n";
        }
        haine_in_masina.clear();
    }
};

void unitTest(){
    /// Haine
    haina A("Pantaloni", 15.3, true);
    haina B("Rochie", 20.3, true);
    haina C("Camasa", 15.3, true);
    haina D("Palton", 7.8, false);
    haina E("Geaca", 15.3, true);
    haina F("Costum", 15.3, true);

    /// Vectori
    vector<haina> haine;
    haine.push_back(A);
    haine.push_back(B);
    haine.push_back(C);
    haine.push_back(D);
    haine.push_back(E);
    haine.push_back(F);
}

void citeste_haine(vector <haina> &haine){
    while(1){
        cout<<"Doriti sa introduceti o haina in sistem? (1 = DA, 2 = NU)\n";
        int caz;
        cin>>caz;
        if(caz == 1){
            cout<<"Introduceti ID-ul clientului, numarul de haine, tipul hainei, greutatea si culoarea articolului (inchisa / deschisa)\n";
            cout<<"  Tipurile de haine acceptate: ";
            for(auto &x : HAINE_POSIBILE)
                cout<<x<<" "; cout<<"\n";

            int id, cantitate;
            string nume;
            double greutate;
            string culoare;

            cin>>id>>cantitate>>nume;
            if(find(begin(HAINE_POSIBILE), end(HAINE_POSIBILE), nume) == HAINE_POSIBILE.end()){
                cout<<"Tipul de haina introdus nu este recunoscut de sistem!\n";
                continue;
            }
            cin>>greutate>>culoare;
            //cout<<"id = "<<id<<"\n"; cout<<"cantitate = "<<cantitate<<"\n"; cout<<"nume = "<<nume<<"\n"; cout<<"greutate = "<<greutate<<"\n"; cout<<"culoare = "<<culoare<<"\n";

            for(int i=0; i<cantitate; ++i){
                haina H(nume, greutate, culoare=="inchisa");
                haine.push_back(H);
            }

            // leaga haina de client...
        }
        else break;
    }
    cout<<"Introducere finalizata!\n";
}

void plaseaza_haine_in_masini(vector<masina> &masini, vector<haina> &haine, int timp){
    for(auto &haina_curenta : haine){ /// Iau fiecare haina
        if(!haina_curenta.haina_se_curata_acum && !haina_curenta.este_calcata) /// Pastrez hainele care asteapta sa fie curatate si care n-au fost curatate inca
        {
            string tip_masina_dorit;
            if(!haina_curenta.este_spalata) tip_masina_dorit = "Spalat";
            else if(!haina_curenta.este_stoarsa) tip_masina_dorit = "Stors";
            else if(!haina_curenta.este_uscata) tip_masina_dorit = "Uscat";
            else if(!haina_curenta.este_calcata) tip_masina_dorit = "Calcat";
            else throw runtime_error("O haina dubioasa incearca sa fie spalata...");

            cout<<"Caut o masina pentru "<<tip_masina_dorit<<"...\n";

            for(auto &masina_curenta : masini) /// Iau fiecare masina
            {
                //cout<<"Tipul masinii: "<<masina_curenta.tip_masina<<"\n";
                if(masina_curenta.masina_poate_efectua_operatia_acestui_tip_de_masina(tip_masina_dorit, haina_curenta)) /// Masina poate spala / stoarce / usca / calca haina, dupa necesitate
                {
                    //cout<<"Masina poate fi folosita!\n";
                    if(masina_curenta.timp_sfarsit_functionare < timp && masina_curenta.haina_are_loc_in_masina(haina_curenta)){ /// Masina este disponibila
                        cout<<"Am adaugat o haina de tip "<<haina_curenta.nume<<" in masina de tip "<<masina_curenta.tip_masina<<"\n";
                        masina_curenta.adauga_haina(haina_curenta);
                    }
                }
            }
        }
    }
}

void scoate_haine_din_masini(vector<masina> &masini, int timp){
    for(auto &masina_curenta : masini){
        cout<<"Incerc sa opresc o masina de tip "<<masina_curenta.tip_masina<<": timp = "<<timp<<", timp_sfarsit = "<<masina_curenta.timp_sfarsit_functionare<<", masina_este_pornita = "<<masina_curenta.este_pornita<<"\n";
        if(masina_curenta.timp_sfarsit_functionare < timp && masina_curenta.este_pornita){
            //cout<<"Scot ceva dintr-o masina de tip "<<masina_curenta.tip_masina<<", timp_sfarsit = "<<masina_curenta.timp_sfarsit_functionare<<"\n";
            masina_curenta.opreste_masina();
            masina_curenta.scoate_hainele_din_masina();
        }
    }
}

void porneste_masinile(vector<masina> &masini, int timp){
    for(auto &masina_curenta : masini)
        if(masina_curenta.poate_fi_pornita(timp))
            masina_curenta.porneste_masina(timp);
}

int main()
{
    freopen("date.in","r",stdin);

    /// Vars
    vector<haina> haine;
    vector<masina> masini;

    masina M1("Spalat", 25, 10);
    M1.adauga_echipament_special();
    masina M2("Stors", 23, 15);
    masina M3("Uscat", 3, 23);
    masina M4("Calcat", 1, 4);

    masini.push_back(M1);
    masini.push_back(M2);
    masini.push_back(M3);
    masini.push_back(M4);

    /// Unit Tests
    unitTest();

    /// Solve
    citeste_haine(haine);
    for(int timp_secunda = 0; timp_secunda <= 10000; timp_secunda += 200){
        cout<<"-------- Ciclu nou de spalare --------\n";
        scoate_haine_din_masini(masini, timp_secunda);
        for(auto &h : haine) h.status();
        plaseaza_haine_in_masini(masini, haine, timp_secunda);
        porneste_masinile(masini, timp_secunda);
    }

    return 0;
}
