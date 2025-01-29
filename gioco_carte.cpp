
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <cmath>

#define BLU "\033[34;1m"
#define ROSSO "\033[31;1m"
#define NERO "\033[30;1m"
#define ARANCIONE "\033[38;5;214m"
#define RESET "\033[0m"

using namespace std;

mt19937 gen(time(nullptr));
uniform_int_distribution<> rr(0, 12);
uniform_int_distribution<> rs(0, 3);
uniform_int_distribution<> rm(0, 51);

enum seme {cuori, picche, quadri, fiori};
enum rango {due, tre, quattro, cinque, sei, sette, otto, nove, dieci, J, Q, K, A};
enum tipo_mazzo {rosso, blu};
enum tipo_punteggio {carta_alta, coppia, doppia_coppia, tris, scala, colore, full, poker, scala_reale, 
                        pokerissimo, full_colore, scala_reale_massima, colore_perfetto};

int adds(int r) {
    switch(r) {
        case 0: return 2;
        case 1: return 3;
        case 2: return 4;
        case 3: return 5;
        case 4: return 6;
        case 5: return 7;
        case 6: return 8;
        case 7: return 9;
        case 8: return 10;
        case 9: return 10;
        case 10: return 10;
        case 11: return 10;
        case 12: return 11;
        default: break;
    }
    return 0;
}

string s_seme(enum seme s) {
    switch(s) {
        case cuori: return "\033[31;1m♥\033[0m";
        case picche: return "\033[30;1m♠\033[0m";
        case quadri: return "\033[38;5;214m♦\033[0m";
        case fiori: return "\033[34;1m♣\033[0m";
        default: break;
    }
    return "?";
}

string s_rango(enum rango r) {
    switch(r) {
        case 0: return "\033[1m2\033[0m";
        case 1: return "\033[1m3\033[0m";
        case 2: return "\033[1m4\033[0m";
        case 3: return "\033[1m5\033[0m";
        case 4: return "\033[1m6\033[0m";
        case 5: return "\033[1m7\033[0m";
        case 6: return "\033[1m8\033[0m";
        case 7: return "\033[1m9\033[0m";
        case 8: return "\033[1m10\033[0m";
        case 9: return "\033[1mJ\033[0m";
        case 10: return "\033[1mQ\033[0m";
        case 11: return "\033[1mK\033[0m";
        case 12: return "\033[1mA\033[0m";
        default: break;
    }
    return "?";
}

string s_tipo(enum tipo_punteggio t) {
    switch(t) {
        case carta_alta: return "Carta Alta";
        case coppia: return "Coppia";
        case doppia_coppia: return "Doppia Coppia";
        case tris: return "Tris";
        case full: return "Full";
        case colore_perfetto: return "Colore Perfetto";
        case scala: return "Scala";
        case colore: return "Colore";
        case scala_reale: return "Scala Reale";
        case scala_reale_massima: return "Scala Reale Massima";
        case poker: return "Poker";
        case pokerissimo: return "Pokerissimo";
        case full_colore: return "Full Colore";
        default: break;
    }
    return "?";
}

string stampa_carta(enum rango r, enum seme s) {
    ostringstream o;
    o << s_rango(r) << " " << s_seme(s);
    return o.str(); 
}

enum tipo_punteggio tp;
double score;
double fiche;
double molt;

int c = 0;

struct Carta {
    enum seme seme;
    enum rango rango;
};

bool confronta_seme(const Carta& c1, const Carta& c2);
bool confronta_rango(const Carta& c1, const Carta& c2);

class Mazzo {
    private:
        vector<Carta> mazzo;
        enum tipo_mazzo tipo;
        int mani, scarti;
        vector<Carta> mano;
        
        void pesca(int n) {
            if (mazzo.size() == 0) {
                cout << "Non ci sono carte rimaste nel mazzo" << endl;
                return;
            }
            shuffle(mazzo.begin(), mazzo.end(), gen);
            for (int i = 0; i < n; i++) {
                if (mazzo.size() == 0) {
                    cout << "Non ci sono carte rimaste nel mazzo" << endl;
                    return;
                }
                Carta c = mazzo.back();
                mazzo.pop_back();
                mano.push_back(c);
            }
        }

        vector<int> index;

        void assegna() {
            switch(tp) {
                case carta_alta: 
                    fiche = 5;
                    molt = 1;
                    break;
                case coppia:
                    fiche = 10;
                    molt = 2;
                    break;
                case doppia_coppia:
                    fiche = 20;
                    molt = 2;
                    break;
                case tris:
                    fiche = 30;
                    molt = 3;
                    break;
                case scala:
                    fiche = 30;
                    molt = 4;
                    break;
                case colore:
                    fiche = 35;
                    molt = 4;
                    break;
                case full:
                    fiche = 40;
                    molt = 4;
                    break;
                case poker:
                    fiche = 60;
                    molt = 7;
                    break;
                case scala_reale:
                    fiche = 100;
                    molt = 8;
                    break;
                case scala_reale_massima:
                    fiche = 120;
                    molt = 9;
                    break;
                case pokerissimo:
                    fiche = 120;
                    molt = 12;
                    break;
                case full_colore:
                    fiche = 140;
                    molt = 14;
                    break;
                case colore_perfetto:
                    fiche = 160;
                    molt = 16;
                    break;
            }
        }

        void punteggio(vector<Carta> giocate) {
            vector<int> num;
            num.resize(13);
            for (size_t i = 0; i < giocate.size(); i++) {
                num[giocate[i].rango]++;
            }

            int max = -1;
            int r_coppia1 = -1;
            int r_coppia2 = -1;
            int r_tris = -1;
            int r_poker = -1;
            int r_pokerissimo = -1;
            bool first = true;
            bool chk = true;
            bool color = false;
            bool scal = false;
            bool s_max = false;

            if (giocate[0].seme == giocate[1].seme and giocate[1].seme == giocate[2].seme 
                and giocate[2].seme == giocate[3].seme and giocate[3].seme == giocate[4].seme) {
                color = true;
            }

            sort(giocate.begin(), giocate.end(), confronta_rango);
            if (((abs(giocate[0].rango - giocate[1].rango) == 1) and (abs(giocate[1].rango - giocate[2].rango) == 1)
                    and (abs(giocate[1].rango - giocate[2].rango) == 1) and (abs(giocate[2].rango - giocate[3].rango) == 1)) 
                or ((giocate[0].rango == A) and (giocate[1].rango == cinque) and (giocate[2].rango == quattro)
                    and (giocate[3].rango == tre) and (giocate[4].rango == due))) {
                if (giocate[0].rango == A and giocate[0].rango != cinque)
                    s_max = true;
                scal = true;
            }

            for (int i = 12; i >= 0; i--) {
                if (num[i] == 5) {
                    r_pokerissimo = i;
                    chk = false;
                }
                
                if (num[i] == 4) {
                    r_poker = i;
                    chk = false;
                }
                
                if (num[i] == 3) { 
                    r_tris = i;   
                    chk = false;
                }         

                if (num[i] == 2) {  
                    if (first) 
                        r_coppia1 = i;
                    else
                        r_coppia2 = i;
                    first = false;
                    chk = false;
                }

                if (num[i] == 1 and i > max and chk) {  
                    max = i;   
                    tp = carta_alta; 
                }
            }
            
            if (scal and color) {
                if (s_max) 
                    tp = scala_reale_massima;
                else 
                    tp = scala_reale;
            } else if (scal) 
                tp = scala;
            
            if (r_coppia1 != -1) {
                if (r_coppia2 != -1)
                    tp = doppia_coppia;
                else if (r_tris != -1)
                    tp = full;
                else
                    tp = coppia;
            } 

            if (r_tris != -1 and r_coppia1 == -1)
                tp = tris;

            if (color) {
                if (tp == full)
                    tp = full_colore;
                else if (r_pokerissimo != -1)
                    tp = colore_perfetto;
                else
                    tp = colore;
            }

            if (r_poker != -1) 
                tp = poker;
            if (r_pokerissimo != -1 and tp != colore_perfetto)
                tp = pokerissimo;

            assegna();

            switch(tp) {
                case carta_alta: 
                    fiche += adds(max);
                    break;
                case coppia:
                    fiche += (adds(r_coppia1) * 2);
                    break;
                case doppia_coppia:
                    fiche += (adds(r_coppia1) * 2) + (adds(r_coppia2) * 2);
                    break;
                case tris:
                    fiche += (adds(r_tris) * 3);
                    break;
                case scala:
                    for (size_t i = 0; i < giocate.size(); i++) {
                        fiche += adds(int(giocate[i].rango));
                    }
                    break;
                case colore:
                    for (size_t i = 0; i < giocate.size(); i++) {
                        fiche += adds(int(giocate[i].rango));
                    }
                    break;
                case full:
                    fiche += (adds(r_tris) * 3) + (adds(r_coppia1) * 2);
                    break;
                case poker:
                    fiche += (adds(r_poker) * 4);
                    break;
                case scala_reale:
                    for (size_t i = 0; i < giocate.size(); i++) {
                        fiche += adds(int(giocate[i].rango));
                    }
                    break;
                case scala_reale_massima:
                    fiche += 51;
                    break;
                case pokerissimo:
                    fiche += (adds(r_pokerissimo) * 5);
                    break;
                case full_colore:
                    fiche += (adds(r_tris) * 3) + (adds(r_coppia1) * 2);
                    break;
                case colore_perfetto:
                    fiche += (adds(r_pokerissimo) * 5);
                    break;
            }
        }

    public: 

        double soldi;

        Mazzo(enum tipo_mazzo type) {
            Carta c;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 13; j++) {
                    c.rango = static_cast<rango>(j);
                    c.seme = static_cast<seme>(i);
                    mazzo.push_back(c);
                }
            }
            if (type == rosso) {
                mani = 4;
                scarti = 4;                
            }
            if (type == blu) {
                mani = 5;
                scarti = 3;
            }
            soldi = 0;
            pesca(8);
        }

        void stampa_mazzo() {
            sort(mazzo.begin(), mazzo.end(), confronta_seme);
            cout << "Mazzo (" << mazzo.size() << "): " << endl;
            for (size_t i = 0; i < mazzo.size(); i++) {
                if (i != 0 and mazzo[i].seme != mazzo[i - 1].seme)
                    cout << endl;
                cout << s_rango(mazzo[i].rango) << " " << s_seme(mazzo[i].seme) << "    ";
            }
            cout << endl;
        }

        void stampa_mano(bool rango) {
            if (rango) sort(mano.begin(), mano.end(), confronta_rango);
            else sort(mano.begin(), mano.end(), confronta_seme);
            cout << NERO << "Mani" << RESET << " = " << BLU << mani << NERO 
                    << "    Scarti" << RESET << " = " << ROSSO << scarti << NERO
                    << "    Soldi" << RESET << " = " << ARANCIONE << soldi << RESET << endl;
            cout << "Mano:    ";
            for (size_t i = 0; i < mano.size(); i++) {
                cout << s_rango(mano[i].rango) << " " << s_seme(mano[i].seme) << "    ";
            }
            cout << endl;
        }

        vector<Carta> seleziona(bool rango) {
            vector<Carta> sel;

            if (mano.size() == 0) 
                return sel;

            index.clear();
            index.resize(0);

            stampa_mano(rango);
            cout << "\033[30m        1       2      3       4      5       6      7       8\033[0m" << endl;
            
            string input;
            int selezionate = 0;

            cout << "\nSi possono selezionare al massimo 5 carte; inserire enter per terminare" << endl;
            cout << "Inserire gli indici delle carte da selezionare (1 - " << mano.size() << "): " << endl;
            
            vector<bool> check;
            check.resize(mano.size() + 1);
            
            while (selezionate < 5) {
                getline(cin, input);
                
                try {
                    if (input.empty())
                        break;
                    size_t indice = stoi(input);
                    if (indice > mano.size() or indice < 1) 
                        break;
                    if (check[indice])
                        break;
                    sel.push_back(mano[indice - 1]);
                    selezionate++;
                    check[indice] = true;
                    index.push_back(indice);
                } catch (const exception& e) {break;}
            }
            
            system("clear");
            cout << "Carte selezionate:    ";
            for (size_t i = 0; i < sel.size(); i++) {
                cout << stampa_carta(sel[i].rango, sel[i].seme) << "    "; 
            }
            return sel;
        }

        void gioca(vector<Carta> giocate) {
            if (giocate.size() <= 0) 
                return;

            punteggio(giocate);
            // AGGIUNGERE SPECIALI, ECC...
            score = fiche * molt;

            for (int i = index.size() - 1; i >= 0; i--) {
                mano.erase(mano.begin() + (index[i] - 1));
            }
            pesca(giocate.size());
            index.clear();
            index.resize(0);

            cout << "\n\n    \033[30;1m" << s_tipo(tp) <<  "\033[0m: \033[34m" << fiche << "\033[0m x \033[31m" << molt << "\033[0m" << endl;
            cout << "Il punteggio della mano è \033[38;5;214m" << score << "\033[0m" <<endl;

            mani--;

            double guadagno = 0;
            guadagno = score / 10;
            cout << "Hai guadagnato " << ARANCIONE << guadagno << RESET << " monete" << endl;
            soldi += guadagno;
        }

        void scarta(vector<Carta> scartate) {
            if (scartate.size() <= 0) 
                return;
            for (int i = index.size() - 1; i >= 0; i--) {
                mano.erase(mano.begin() + (index[i] - 1));
            }
            pesca(scartate.size());
            index.clear();
            index.resize(0);
            scarti--;
        }

        int get_scarti() {
            return scarti;
        }

        int get_mani() {
            return mani;
        }
};

bool confronta_seme(const Carta& c1, const Carta& c2) {
    if (c1.seme != c2.seme) 
        return c1.seme < c2.seme;
    return c1.rango > c2.rango;
}

bool confronta_rango(const Carta& c1, const Carta& c2) {
    if (c1.rango != c2.rango) 
        return c1.rango > c2.rango;
    return c1.seme < c2.seme;
}

void menu(Mazzo m) {
    system("clear");
    m.stampa_mano(true);
    vector<Carta> vv;
    bool rrg = true;
    while (true) {
        cout <<"\033[35m\nOpzioni: \n[1] Stampa Mazzo       [2] Stampa mano (rango)" 
                << "       [3] Stampa mano (seme)\n[4] Seleziona carte       [5] Scarta       [6] Gioca       [7] Termina" << RESET << endl;
        int scelta = 0;
        cin >> scelta;
        
        cout << endl;

        switch(scelta) {
            case 1: system("clear"); m.stampa_mazzo(); break;
            case 2: system("clear"); m.stampa_mano(true); rrg = true; break;
            case 3: system("clear"); m.stampa_mano(false); rrg = false; break;
            case 4: 
                system("clear");
                vv.clear();
                vv.resize(0);
                while((c = getchar()) != '\n' && c != EOF);
                if (rrg)
                    vv = m.seleziona(true); 
                else 
                    vv = m.seleziona(false);
                break;
            case 5: 
                system("clear");
                if (m.get_scarti() <= 0) {
                    cout << "Scarti finiti" << endl;
                    continue;
                } else if (vv.size() == 0) {
                    cout << "Selezionare delle carte" << endl;
                    continue;
                } else 
                    m.scarta(vv);
                vv.clear();
                vv.resize(0);
                break;
            case 6:
                system("clear");
                if (m.get_mani() <= 0) {
                    cout << "Mani finite" << endl;
                    continue;
                } else if (vv.size() == 0) {
                    cout << "Selezionare delle carte" << endl;
                    continue;
                } else 
                    m.gioca(vv);
                vv.clear();
                vv.resize(0);
                break;
            case 7: 
                system("clear");
                cout << "\nHai guadagnato in tutto " << ARANCIONE << m.soldi << RESET << " monete!\n" << endl;
                return;
            default: break;
        }
    }
}

int main() {
    system("chcp 65001");
    system("clear");
    cout << "           [1] Mazzo Rosso                  [2] Mazzo Blu" << endl;
    int sc;
    cin >> sc;
    if (sc == 1) {
        Mazzo m1(rosso);
        menu(m1);
        return 0;
    }
    Mazzo m2(blu);
    menu(m2);
    return 0;
}
