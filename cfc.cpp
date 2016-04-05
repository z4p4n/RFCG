#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstdlib>

using namespace std;
const int NBN = 1000+1;
 
vector <int> fils[NBN], pere[NBN], triTopo, compCour, isolatedCfc;
bool dejaVu[NBN];
int nbN, nbA, nbCfc;

vector <vector <int> > cfc;
int numCfc[NBN];

vector <int> edgeCfc[NBN];
int nIncoming[NBN]; //nombre d'arete qui arrivent vers une cfc


/*-----------------------------------*/
/* condensation du graphe*/

void parcTrans(int noeud)
{
    if(dejaVu[noeud])
        return;
    dejaVu[noeud] = true;
    for(unsigned i = 0; i < pere[noeud].size(); i++)
        parcTrans(pere[noeud][i]);
    compCour.push_back(noeud);
    numCfc[noeud] = nbCfc;
}
 
void triTop(int noeud)
{
    if(dejaVu[noeud])
        return;
    dejaVu[noeud] = true;
    for(unsigned i = 0; i < fils[noeud].size(); i++)
        triTop(fils[noeud][i]);
    triTopo.push_back(noeud);
}



void condense()
{
    bool dejaPris[nbCfc];
    for(int idC = 0; idC < nbCfc; idC++)
    {
        fill(dejaPris, dejaPris + nbCfc, false);
        dejaPris[idC] = true;
        //printf("idC = %d, taille = %lu\n", idC, cfc[idC].size());

        for(unsigned nd = 0; nd < cfc[idC].size(); nd++)
            for(unsigned idFils = 0; idFils < fils[ cfc[idC][nd] ].size(); idFils++)
            {
                //printf("\t%d->%d\n", numCfc[ cfc[idC][nd] ], numCfc [fils[ cfc[idC][nd] ][idFils] ]);
                if(!dejaPris[ numCfc [fils[ cfc[idC][nd] ][idFils] ] ])
                {
                    edgeCfc[idC].push_back(numCfc [fils[ cfc[idC][nd] ][idFils] ]);
                    nIncoming[ numCfc [fils[ cfc[idC][nd] ][idFils] ] ]++;
                    dejaPris[numCfc [fils[ cfc[idC][nd] ][idFils] ] ] = true;
                }
             }
        //printf("\n");
    }
}

/*-----------------------------------*/
/*On retourne s'il y a plus de sources que de puits*/

bool isReturned = false;
int nSource = 0, nSink = 0;

void returnGraph()
{
    for(unsigned iN = 0; iN < nbCfc; iN++)
    {
        if(edgeCfc[iN].size() == 0 && nIncoming[iN] == 0)
            isolatedCfc.push_back(iN);
        else if(nIncoming[iN] == 0)
            nSource++;
        else if(edgeCfc[iN].size() == 0) 
            nSink++;
    }

    if(nSource > nSink)
    {
        isReturned = true;
        vector < vector <int> > edgeCfc_temp;
        
        for(unsigned i = 0; i < nbCfc; i++)
            edgeCfc_temp.push_back(edgeCfc[i]);
        
        for(unsigned i = 0; i < nbCfc; i++)
            edgeCfc[i].clear();

        for(unsigned d = 0; d < nbCfc; d++)
        {
            for(unsigned a = 0; a < edgeCfc_temp[d].size(); a++)
                edgeCfc[ edgeCfc_temp[d][a] ].push_back(d);
            nIncoming[d] = edgeCfc_temp[d].size();
        }
    }

/*    printf("Apres retournement, graphe :\n\n");
    for(int i = 0; i < nbCfc; i++)
    {
        printf("noeud %d, fils : ", i);
        for(unsigned j = 0; j < edgeCfc[i].size(); j++)
            printf("%d ", edgeCfc[i][j]);
        printf("\n");
    }
*/
}


/*-----------------------------------*/
/* recherche des sources & puits a connecter*/

vector <bool> marked;
const int UNDEF = -1;

int search(int node)
{
    if(marked[node])
        return UNDEF;
    marked[node] = true;
    if(edgeCfc[node].size() == 0) // node is a sink
        return node;

    for(unsigned nxt = 0; nxt < edgeCfc[node].size(); nxt++)
    {
        int w = search(edgeCfc[node][nxt]);
        if(w != UNDEF) return w;
    }
    return UNDEF;
}


vector <int> sink, source;
unsigned p = 0;

void ST()
{

    marked.assign(nbCfc, false); // initialiser un vector de taille nbCfc à false
    vector <int> nxtSources;
    //printf("nbCfc = %d\n", nbCfc);
    
    for(int iSource = 0; iSource < nbCfc; iSource++)
        if(nIncoming[iSource] == 0 && edgeCfc[iSource].size() != 0) // source qu'on a pas deja vue
        {
            //printf("iSource : %d\n", iSource);
            int iSink = search(iSource);
            if(iSink != UNDEF)
            {
                source.push_back(iSource);
                sink.push_back(iSink);
            }
            else
                nxtSources.push_back(iSource);
        }

    p = source.size();
    source.insert(source.end(), nxtSources.begin(), nxtSources.end());
    
    //printf("\n");
    for(int iSink = 0; iSink < nbCfc; iSink++)
        if(!marked[iSink] && edgeCfc[iSink].size() == 0 && nIncoming[iSink] != 0) // puits qu'on a pas vu
            sink.push_back(iSink);
    
    /*printf("Sources :");
    for(int i = 0; i < source.size(); i++)
        printf("%d ", source[i]);
    printf("\n");
    */
}


/*-----------------------------------*/
/* creation du nouveau graphe */


//TODO compter les composantes connexes toutes seules (le q de l'article)
vector < pair <int, int> > edgeAdded;

void addEdgeInCfc()
{
    int s = source.size(), t = sink.size(), q = isolatedCfc.size();
    //printf("s = %d, t =  %d, p = %d, q = %d\n", s, t, p, q); 
    /*printf("sources :");
    for(int i = 0; i < s; i++) printf(" %d", source[i]);
    printf("\n");
    printf("puits :");
    for(int i = 0; i < t; i++) printf(" %d", sink[i]);
    printf("\n");
    */
    for(unsigned i = 1; i < p; i++)
        edgeAdded.push_back(pair <int, int> (sink[i-1], source[i]));
    
    for(unsigned i = p+1; i <= s; i++)
        edgeAdded.push_back(pair <int, int> (sink[i-1], source[i-1]));
    
    if(q == 0)
    {    
        if(s < t)
        {
            edgeAdded.push_back( pair <int, int> (sink[p-1], sink[s]));
        
            for(unsigned i = s+1; i < t; i++)
                edgeAdded.push_back(pair <int, int> (sink[i-1], sink[i]));

            edgeAdded.push_back( pair <int, int> (sink[t-1], source[0]));
        }
        else
            edgeAdded.push_back( pair <int, int> (sink[p-1], source[0]));
    }

    else
    {
        for(unsigned i = s+1; i < t; i++)
            edgeAdded.push_back(pair <int, int> (sink[i-1], sink[i]));
        
        for(unsigned i = 1; i < q; i++)
            edgeAdded.push_back(pair <int, int> (isolatedCfc[i-1], isolatedCfc[i]));
        
        if(s != 0) //=> q != 0 aussi
        {
            edgeAdded.push_back(pair <int, int> (sink[t-1], isolatedCfc[0]));
            edgeAdded.push_back(pair <int, int> (isolatedCfc[q-1], source[0]));        
            edgeAdded.push_back(pair <int, int> (sink[p-1], source[s]));
        }
        else if (q > 1) //on a plusieurs noeuds isolés
            edgeAdded.push_back(pair <int, int> (isolatedCfc[q-1], isolatedCfc[0]));
    }
}

void addEdge()
{
    addEdgeInCfc();
    for(unsigned iEd = 0; iEd < edgeAdded.size(); iEd++)    
    {
        int d = cfc[ edgeAdded[iEd].first ][0];
        int a = cfc[ edgeAdded[iEd].second ][0];
        
        if(!isReturned)
            fils[d].push_back(a);
        else
            fils[a].push_back(d);
    }
}

int main()
{
    scanf("%d%d", &nbN, &nbA);
    for(int i = 0; i < nbA; i++)
    {
        int d, a;
        scanf("%d%d", &d, &a);
        fils[d].push_back(a);
        pere[a].push_back(d);
    }
    
    for(int i = 0; i < nbN; i++)
        triTop(i);
       
    fill(dejaVu, dejaVu+nbN+1, false);
    
    for(int i = triTopo.size() - 1; i >= 0; i--)
        if(!dejaVu[triTopo[i]])
        {
            compCour.clear();
            parcTrans(triTopo[i]);
            cfc.push_back(compCour);
            nbCfc++;
        }
    
    condense();
    returnGraph();
    ST();
    addEdge();
    
    for(int i = 0; i < nbN; i ++)
    {
        printf("noeud %d, fils : ", i);
        for(int j = 0; j < fils[i].size(); j++)
            printf("%d ", fils[i][j]);
        printf("\n");
    }
}
