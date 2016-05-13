#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int NBN = 1000+1;

vector < vector <int> > fils;
vector <bool> dejaVu;

int creerNoeud(int fils)
{
    vector <int> nvNd;
    nvNd.push_back(fils);

    fils.push_back(nvNd);
    dejaVu.push_back(false);
    return dejaVu.size() - 1;
}

/*apres le traitement : fils[noeud][0] est le suivant dans l'ordre asm */
/*si un seul fils : c'est qu'il est situé juste apres */

void linearize(int noeud)
{
    if(dejaVu[noeud] = true) return;

    dejaVu[noeud] = true;

    if(fils[noeud].size() == 0) return;
    if(fils[noeud].size() == 1)
    {
        if(dejaVu[ fils[noeud][0] ])
        {
            int ndIntermediaire = creerNoeud(fils[noeud][0]);
            fils[noeud][0] = nbIntermediaire;
        }
        linearize(fils[noeud][0]);
    }

    else if(fils[noeud].size() == 2)
    {
        if(!dejaVu[ fils[noeud][1] ])
            swap(fils[noeud][0], fils[noeud][1]);
        else
        {
            int ndIntermediaire = creerNoeud(fils[noeud][0]);
            fils[noeud][0] = ndIntermediaire;
        }
        linearize(fils[noeud][0]);
        linearize(fils[noeud][1]);
    }
    
    else
    {
        //trouver un noeud dejaVu, le mettre en 1
        for(unsigned i = 0; i < fils[noeud].size(); i++)
            if(dejaVu[ fils[noeud][i] ])
            {
                swap(fils[noeud][1], fils[noeud][i]);
                break;
            }

        //mettre en 0 un nvNoeud qui a tous les autres
        int ndIntermediaire = creedNoeud(fils[noeud][0]);
        for(unsigned i = 2; i < fils[noeud].size(); i++)
            fils[ndIntermediaire].push_back(fils[noeud][i]);

        fils[noeud][0] = ndIntermediaire;
        fils[noeud].resize(2);
        
        //appeler sur 0, puis sur 1
        linearize(fils[noeud][0]);
        linearize(fils[noeud][1]);
    }
    
}
int main()
{
    return 0;
}
