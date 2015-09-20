#include "btree.h"

bool Btree::insert(dtype x)
{  node *pNew;
    dtype xNew;
    status code = ins(root, x, xNew, pNew);
    if (code == DuplicateKey)
        cout << "Duplicate key ignored.\n";
    if (code == InsertNotComplete)
    {  node *root0 = root;
        root = new node;
        root->n = 1; root->k[0] = xNew;
        root->p[0] = root0; root->p[1] = pNew;
        return true;
    }
    return false;
}

status Btree::ins(node *r, dtype x, dtype &y, node* &q)
{  // Insert x in *this. If not completely successful, the
    // integer y and the pointer q remain to be inserted.
    // Return value:
    //    Success, DuplicateKey or InsertNotComplete.
    node *pNew, *pFinal;
    int i, j, n;
    dtype xNew, kFinal;
    status code;
    if (r == NULL){q = NULL; y = x; return InsertNotComplete;}
    n = r->n;
    i = NodeSearch(x, r->k, n);
    if (i < n && x == r->k[i]) return DuplicateKey;
    code = ins(r->p[i], x, xNew, pNew);
    if (code != InsertNotComplete) return code;
    // Insertion in subtree did not completely succeed;
    // try to insert xNew and pNew in the current node:
    if (n < M - 1)
    {  i = NodeSearch(xNew, r->k, n);
        for (j=n; j>i; j--)
        {  r->k[j] = r->k[j-1]; r->p[j+1] = r->p[j];
        }
        r->k[i] = xNew; r->p[i+1] = pNew; ++r->n;
        return Success;
    }
    // Current node is full (n == M - 1) and will be split.
    // Pass item k[h] in the middle of the augmented
    // sequence back via parameter y, so that it
    // can move upward in the tree. Also, pass a pointer
    // to the newly created node back via parameter q:
    if (i == M - 1) {kFinal = xNew; pFinal = pNew;} else
    {  kFinal = r->k[M-2]; pFinal = r->p[M-1];
        for (j=M-2; j>i; j--)
        {  r->k[j] = r->k[j-1]; r->p[j+1] = r->p[j];
        }
        r->k[i] = xNew; r->p[i+1] = pNew;
    }
    int h = (M - 1)/2;
    y = r->k[h];             // y and q are passed on to the
    q = new node;            // next higher level in the tree
    // The values p[0],k[0],p[1],...,k[h-1],p[h] belong to
    // the left of k[h] and are kept in *r:
    r->n = h;
    // p[h+1],k[h+1],p[h+2],...,k[M-2],p[M-1],kFinal,pFinal
    // belong to the right of k[h] and are moved to *q:
    q->n = M - 1 - h;
    for (j=0; j < q->n; j++)
    {  q->p[j] = r->p[j + h + 1];
        q->k[j] = (j < q->n - 1 ? r->k[j + h + 1] : kFinal);
    }
    q->p[q->n] = pFinal;
    return InsertNotComplete;
}

void Btree::pr(const node *r, int nSpace)const
{  if (r)
    {  int i;
        cout << setw(nSpace) << "";
        for (i=0; i < r->n; i++)
            cout << setw(3) << r->k[i] << " ";
        cout << endl;
        for (i=0; i <= r->n; i++) pr(r->p[i], nSpace+8);
    }
}

int Btree::NodeSearch(dtype x, const dtype *a, int n)const
{  int i=0;
    while (i < n && x > a[i]) i++;
    return i;
}

void Btree::ShowSearch(dtype x)const
{  cout << "Search path:\n";
    int i, j, n;
    node *r = root;
    while (r)
    {  n = r->n;
        for (j=0; j<r->n; j++) cout << " " << r->k[j];
        cout << endl;
        i = NodeSearch(x, r->k, n);
        if (i < n && x == r->k[i])
        {  cout << "Key " << x << " found in position " << i
                << " of last displayed node.\n";
            return;
        }
        r = r->p[i];
    }
    cout << "Key " << x << " not found.\n";
}

bool Btree::DelNode(dtype x)
{  node *root0;
    switch (del(root, x))
    {
    case NotFound:
        cout << x << " not found.\n";
        break;
    case Underflow:
        root0 = root;
        root = root->p[0];
        delete root0;
        return true;
    }
    return false;
}

status Btree::del(node *r, dtype x)
{  if (r == NULL) return NotFound;
    int i, j, pivot, n = r->n;
    dtype *k = r->k;  // k[i] means r->k[i]
    const int nMin = (M - 1)/2;
    status code;
    node **p = r->p, *pL, *pR;       // p[i] means r->p[i]
    i = NodeSearch(x, k, n);
    if (p[0] == NULL) // *r is a leaf
    {  if (i == n || x < k[i]) return NotFound;
        // x == k[i], and *r is a leaf
        for (j=i+1; j < n; j++)
        {  k[j-1] = k[j]; p[j] = p[j+1];
        }
        return
                --r->n >= (r==root ? 1 : nMin) ? Success : Underflow;
    }
    // *r is an interior node, not a leaf:
    if (i < n && x == k[i])
    {  // x found in an interior node. Go to left child
        // *p[i] and follow a path all the way to a leaf,
        // using rightmost branches:
        node *q = p[i], *q1; int nq;
        for (;;)
        {  nq = q->n; q1 = q->p[nq];
            if (q1 == NULL) break;
            q = q1;
        }
        // Exchange k[i] (= x) with rightmost item in leaf:
        k[i] = q->k[nq-1];
        q->k[nq - 1] = x;
    }
    // Delete x in leaf of subtree with root p[i]:
    code = del(p[i], x);
    if (code != Underflow) return code;
    // There is underflow; borrow, and, if necessary, merge:
    // Too few data items in node *p[i]
    if (i > 0 && p[i-1]->n > nMin) // Borrow from left sibling
    {  pivot = i - 1; // k[pivot] between pL and pR:
        pL = p[pivot]; pR = p[i];
        // Increase contents of *pR, borrowing from *pL:
        pR->p[pR->n + 1] = pR->p[pR->n];
        for (j=pR->n; j>0; j--)
        {  pR->k[j] = pR->k[j-1];
            pR->p[j] = pR->p[j-1];
        }
        pR->n++;
        pR->k[0] = k[pivot];
        pR->p[0] = pL->p[pL->n];
        k[pivot] = pL->k[--pL->n];
        return Success;
    }
    if (i<n && p[i+1]->n > nMin) // Borrow from right sibling
    {  pivot = i; // k[pivot] between pL and pR:
        pL = p[pivot]; pR = p[pivot+1];
        // Increase contents of *pL, borrowing from *pR:
        pL->k[pL->n] = k[pivot];
        pL->p[pL->n + 1] = pR->p[0];
        k[pivot] = pR->k[0];
        pL->n++; pR->n--;
        for (j=0; j < pR->n; j++)
        {  pR->k[j] = pR->k[j+1];
            pR->p[j] = pR->p[j+1];
        }
        pR->p[pR->n] = pR->p[pR->n + 1];
        return Success;
    }
    // Merge; neither borrow left nor borrow right possible.
    pivot = (i == n ? i - 1 : i);
    pL = p[pivot]; pR = p[pivot+1];
    // Add k[pivot] and *pR to *pL:
    pL->k[pL->n] = k[pivot];
    pL->p[pL->n + 1] = pR->p[0];
    for (j=0; j < pR->n; j++)
    {  pL->k[pL->n + 1 + j] = pR->k[j];
        pL->p[pL->n + 2 + j] = pR->p[j+1];
    }
    pL->n += 1 + pR->n;
    delete pR;
    for (j=i+1; j < n; j++)
    {  k[j-1] = k[j]; p[j] = p[j+1];
    }
    return
            --r->n >= (r == root ? 1 : nMin) ? Success : Underflow;
}

// Retorna la altura del arbol, empieza en 0, i = -1
int Btree :: altura(node * root, int i)
{
    if(!root)
    {
        cout<<"NULL"<<endl;
        return i;
    }
    else
    {
        cout<<root->k[0]<<"("<<root->n<<") - ";
        i++;
        altura(root->p[0], i);
    }
}

// Retorna cuantas Hojas hay (Nodos sin hijos), empieza en 0, j y l = -1
int Btree :: numHojas(node *root, int j, int l)
{
    if(!root)
    {
        return j;
    }
    else
    {
        for(int i = 0; i <= root->n ; i++)
        {
            if(l > j) j = l;
            l = numHojas(root->p[i], j++, l);
        }
    }

    return l;
}

QString Btree :: buscar(dtype x)
{
    QString sr;
    sr.clear();

    int i, j, n;
    node *r = root;
    while (r)
    {  n = r->n;
        for (j=0; j<r->n; j++)
        {
            sr.append(" ");
            sr.append(QString::number(r->k[j]));
        }
        sr.append(" -> ");

        i = NodeSearch(x, r->k, n);
        if (i < n && x == r->k[i])
        {
            return sr;
        }
        r = r->p[i];
    }
    sr.append("No se encontro: ");
    sr.append(QString::number(x));

    return sr;
}

void Btree :: printTree(node * rt, int x, int y)
{
    if(!rt)
        return;

    else
    {

        scene->addRect(QRectF(320, 81, 25, 25));
        for(int i = 0; i < y ; i++)
        {
            int j = 0;
            while (rt->p[j])
            {
                if(j % 2 == 0)
                    scene->addRect(QRectF(320+(x*25), 81+(y*25), 25, 25));
                else
                    scene->addRect(QRectF(320-(x*25), 81+(y*25), 25, 25));
                j++;
            }

            rt = rt->p[i];
        }


        //    scene->addRect(QRectF(310+x, 71+y, 25, 25));

        //    for(int i = 0; i < rt->n; i++)
        //        printTree(rt->p[i], (x+1)*25, (y+1)*25);
    }

}

void Btree :: play()
{  cout <<
           "B-tree structure shown by indentation. For each\n"
           "node, the number of links to other nodes will not\n"
           "be greater than " << M <<
           ", the order M of the B-tree.\n\n"
           "Enter some integers, followed by a slash (/):\n";
    Btree t;
    dtype x;
    char ch;
    while (cin >> x, !cin.fail()) t.insert(x);
    cout <<
            "\nB-tree representation (indentation similar to the\n"
            "table of contents of a book). The items stored in\n"
            "each node are displayed on a single line.\n";
    t.print();
    cin.clear(); cin >> ch; // Skip terminating character
    for (;;)
    {  cout <<
               "\nEnter an integer, followed by I, D, or S (for\n"
               "Insert, Delete and Search), or enter Q to quit: ";
        cin >> x >> ch;
        if (cin.fail()) break;
        ch = toupper(ch);
        switch(ch)
        {
        case 'S': t.ShowSearch(x); break;
        case 'I': t.insert(x); break;
        case 'D': t.DelNode(x); break;
        default:
            cout << "Invalid command, use S, I or D\n"; break;
        }
        if (ch == 'I' || ch == 'D') t.print();
    }

    cout<<"-------------------------------------------------------"<<endl;

    cout<<"Altura: "<<altura(t.root, -1)<<endl;
    cout<<"----------------------------------"<<endl;
    cout<<"Num Hojas: "<<numHojas(t.root, -1, -1)<<endl;

}
