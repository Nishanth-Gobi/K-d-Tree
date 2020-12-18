// Nearest Neighbors Search using K-d Tree
// By Nishanth Gobi

#include <iostream>
#include <math.h>

using namespace std;

#define DIMENSION 3

const int k = DIMENSION;

class node
{
public:
    int point[k];
    node* lchild;
    node* rchild;
    node()
    {
        lchild = NULL;
        rchild = NULL;
    }
    void show()
    {
        int i;
        cout<<"(";
        for(i=0;i<k;i++)
        {
            cout<<point[i]<<" ";
        }
        cout<<")";
    }
};

void new_node(node* n, int a[])
{
    int i;
    for(i=0;i<k;i++)
    {
        n->point[i] = a[i];
    }
}

node* root = new node();

node* initiate_tree(int a[]) // To build a new k-d tree
{
    new_node(root, a);
}

void insert_node(int a[]) // Insert function for k-d tree
{
    node* p = root;
    int d=0, cd=0;
    while(1)
    {
        cd = d%k;
        if(a[cd]>=p->point[cd])
        {
            if(p->rchild==NULL)
            {
                node* temp = new node();
                new_node(temp, a);
                p->rchild = temp;
                break;
            }
            else
            {
                p = p->rchild;
            }
        }
        else if(a[cd]<p->point[cd])
        {
            if(p->lchild==NULL)
            {
                node* temp = new node();
                new_node(temp, a);
                p->lchild = temp;
                break;
            }
            else
            {
                p = p->lchild;
            }
        }
        d++;
    }
}

node* min_node(node* a, node* b, node* c, int n) //to be used with delete_element
{
    node* r = a;
    if(b != NULL && b->point[n] < r->point[n])
    {
        r = b;
    }
    if(c != NULL && c->point[n] < r->point[n])
    {
        r = c;
    }
    return r;
}

node* find_min(node* p, int n, int d=0) //to be used with delete_element
{
    if(p==NULL)
    {
        return NULL;
    }
    int cd = d%k;
    if(cd == n)
    {
        if(p->lchild == NULL)
        {
            return p;
        }
        return find_min(p->lchild, n, d+1);
    }
    return min_node(p, find_min(p->lchild, n, d+1), find_min(p->rchild, n, d+1), n);
}

void replacement(int a[], int b[]) //to be used with delete_element
{
    int i;
    for(i=0;i<k;i++)
    {
        a[i] = b[i];
    }
}

node* delete_element(node *p, int a[], int d=0) // Delete function for k-d tree
{
    if(p == NULL)
    {
        return NULL;
    }
    int i, cd = d%k, fl = 0;
    for(i=0;i<k;i++)
    {
        if(a[i]!=p->point[i])
        {
            fl = -1;
        }
    }
    if(fl==0)
    {
        if(p->rchild != NULL)
        {
            node *m = find_min(p->rchild, cd);
            replacement(p->point, m->point);
            p->rchild = delete_element(p->rchild, m->point, d+1);
        }
        else if(p->lchild != NULL)
        {
            node *m = find_min(p->lchild, cd);
            replacement(p->point, m->point);
            p->lchild = delete_element(p->lchild, m->point, d+1);
            p->rchild = p->lchild;
            p->lchild = NULL;
        }
        else
        {
            delete p;
            return NULL;
        }
        return p;
    }
    if(a[cd] < p->point[cd])
    {
        p->lchild = delete_element(p->lchild, a, d+1);
    }
    else
    {
        p->rchild = delete_element(p->rchild, a, d+1);
    }
    return p;
}

bool search_node(int a[]) // search function for k-d tree
{
    node* p = root;
    int i, d=0, cd=0, fl;
    while(p!=NULL)
    {
        fl = 0;
        for(i=0;i<k;i++)
        {
            if(p->point[i]!=a[i])
            {
                fl = -1;
            }
        }
        if(fl==0)
        {
            return true;
        }

        cd = d%k;
        if(p->point[cd]>a[cd])
        {
            p = p->lchild;
        }
        else
        {
            p = p->rchild;
        }
        d++;
    }
    return false;
}

void display(node* p) //in-order traversal
{
    if(p!=NULL)
    {
        display(p->lchild);
        p->show();
        display(p->rchild);
    }
}

float sq_dis(node* a, node* b) //distance function to be used with nns
{
    int i;
    float sum = 0;
    for(i=0;i<k;i++)
    {
        sum += (a->point[i] - b->point[i])*(a->point[i] - b->point[i]);
    }
    return sum;
}

node* closest(node* a, node* b, node* c) //closest point function to be used with nns
{
    int i;
    float d1 = 0, d2 = 0;
    for(i=0;i<k;i++)
    {
        d1 += (a->point[i] - b->point[i])*(a->point[i] - b->point[i]);
    }
    for(i=0;i<k;i++)
    {
        d2 += (a->point[i] - c->point[i])*(a->point[i] - c->point[i]);
    }
    if(d1>d2)
    {
        return c;
    }
    return b;
}


node* nns(node* r, node* a, int n) //Nearest Neighbor search function
{
    node* next = new node();
    node* other = new node();

    if(r==NULL)
    {
        return NULL;
    }
    if(a->point[n%k] < r->point[n%k])
    {
        next = r->lchild;
        other = r->rchild;
    }
    else
    {
        next = r->rchild;
        other = r->lchild;
    }
    node* temp = nns(next, a, n+1);
    node* best = r;

    if (temp!=NULL)
    {
        best = closest(a,temp, r);
    }
    float distsq = sq_dis(a ,best);
    int fl = a->point[n%k] - r->point[n%k];

    if(distsq >= fl*fl)
    {
        temp = nns(other, a, n+1);
        if(temp!=NULL)
        {
            best = closest(a,temp,best);
        }
    }
    return best;
}

node* k_neighbors(int a[], int m) //Finding m-nearest Nearest Neighbor elements
{
    node* p = new node();
    new_node(p, a);
    int i, j, ar[m][k];
    for(i=0;i<m;i++)
    {
        node* ans = nns(root,p,0);
        cout<<"\nNeighbor: "<<i+1<<": ";
        ans->show();
        cout<<" ,Squared Distance: "<<sq_dis(ans,p);
        for(j=0;j<k;j++)
        {
            ar[i][j] = ans->point[j];
        }
        delete_element(root, ar[i]);
    }
    for(i=0;i<m;i++)
    {
        insert_node(ar[i]);
    }
    cout<<endl<<endl;
}

int main()
{
    cout<<"\t\t\t\t\tNearest neighbor search using K-d Tree"<<endl<<endl<<endl;
    cout<<"Enter dimension value(K): ";

    cout<<"K-d Tree creation:"<<endl<<endl;

    int n=7, i, j,choice;
    cout <<"Enter your choice:\n 1)Sample Simulation\n 2)Custom Input\n\t(1/2)--> ";
    while(1)
    {
        cin>>choice;
        if(choice==1 || choice==2)
        {
            break;
        }
        cout<<"\nRe-enter a valid choice: ";
    }
    int temp[k];
    if(choice==1)
    {
        int sample[7][k] = {{3, 6}, {17, 15}, {13, 15}, {6, 12},{9, 1}, {2, 7}, {10, 19}};
        for(i=0;i<7;i++)
        {
                for(j=0;j<k;j++)
                {
                        temp[j] = sample[i][j];
                }
                if(i==0)
                {
                        initiate_tree(temp);
                }
                else
                {
                        insert_node(temp);
                }
        }
    }
    else
    {
            cout<<"Enter the number of points: ";
            cin>>n;
            int sample[20][k];
            cout<<"\nEnter the values: \n";
            for(i=0;i<n;i++)
            {
                    cout<<"Point: "<<i+1<<" ";
                    for(int j=0;j<k;j++)
                    {
                            cin>>sample[i][j];
                    }
            }
            for(i=0;i<n;i++)
            {
                for(j=0;j<k;j++)
                {
                        temp[j] = sample[i][j];
                }
                if(i==0)
                {
                        initiate_tree(temp);
                }
                else
                {
                        insert_node(temp);
                }

             }
    }
    cout<<endl<<"\t\t\t\t\t***Constructed K-d Tree***"<<endl<<endl;
    display(root);
    cout<<endl;
    int resp;
    int val[2];
    while(1)
    {
        cout<<"\n1)Insert element\n2)Display\n3)Search element\n4)Nearest neighbor\n5)Delete element\n6)M Nearest neighbor\n7)Exit\n\t\t(1/2/3/4/5/6/7): ";
        cin>>resp;
        if(resp==1)
        {
            cout<<"Enter data for the element: ";
            for(j=0;j<k;j++)
            {
                cin>>val[j];
            }
            insert_node(val);
        }
        else if(resp==2)
        {
            display(root);
            cout<<"\n";
        }
        else if(resp==3)
        {
            cout<<"Enter data for the element: ";
            for(j=0;j<k;j++)
            {
                cin>>val[j];
            }
            if(search_node(val))
            {
                cout<<"\nPoint found in tree\n";
            }
            else
            {
                cout<<"\nPoint not found in tree\n";
            }
        }
        else if(resp==4)
        {
            cout<<"Enter data for the element: ";
            for(j=0;j<k;j++)
            {
                cin>>val[j];
            }
            node* p = new node();
            new_node(p,val);
            node* ans = nns(root,p,0);
            cout<<"\nNearest neighbor node: ( ";
            for(i=0;i<k;i++)
            {
                cout<<ans->point[i]<<" ";
            }
            cout<<")"<<endl;
            cout<<"Squared Distance: "<<sq_dis(ans,p)<<endl<<endl;
        }
        else if(resp==5)
        {
            cout<<"Enter data for the element: ";
            for(j=0;j<k;j++)
            {
                cin>>val[j];
            }
            delete_element(root, val, 0);
        }
        else if(resp==6)
        {
            int c;
            cout<<"Enter data for the element: ";
            for(j=0;j<k;j++)
            {
                cin>>val[j];
            }
            cout<<"\nEnter the number of neighboring points: ";
            while(1)
            {
                cin>>c;
                if(c<=n && c>0)
                {
                    break;
                }
                cout<<"\nRe-enter a valid count: ";
            }
            k_neighbors(val, c);
        }
        else
        {
            break;
        }
    }
    return 0;
}
//The End
