
//#include <lib.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <fstream>
using namespace std;

#define MAX 100
#define ROOT 0

// Global Variables

int totalNodesCreated=0;
queue<int> levelQ;

class Node{

    public:
        vector <vector<int> > block;
        int idNum;
        int parentNum;
        int cost;
        tuple<int, int> currBlankPos;
        tuple<int, int> prevBlankPos;

        void callActions(int idx, vector<vector <int> > data);
        void addNode(Node& newNode, vector <vector <int> > data, int id, int pNum);
        tuple<int, int> posBlankTile(vector <vector <int> > data);
        void actionMoveLeft(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old);
        void actionMoveRight(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old);
        void actionMoveUp(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old);
        void actionMoveDown(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old);
        bool checkDuplicate(vector<vector <int> > data);
};

Node nodesInfo[MAX];     // Array of objects of the class Node


void generateNodes(){
    while(!levelQ.empty() && totalNodesCreated<MAX){
        int idx = levelQ.front();
        nodesInfo[idx-1].callActions(idx, nodesInfo[idx-1].block);
        //cout<<"Size of queue : "<<levelQ.size()<<endl;
        levelQ.pop();
        //cout<<"Size of queue after pop: "<<levelQ.size()<<endl;
    }
}

// members of class Node

void Node::callActions(int idx, vector <vector <int> > data){       // idx - parent , so now we will make the child nodes from this parent
    int x = get<0>(nodesInfo[idx-1].currBlankPos);
    int y = get<1>(nodesInfo[idx-1].currBlankPos);
    int x_old = get<0>(nodesInfo[idx-1].prevBlankPos);
    int y_old = get<1>(nodesInfo[idx-1].prevBlankPos);

    actionMoveLeft(idx, data,x, y, x_old, y_old);
    actionMoveRight(idx, data,x, y, x_old, y_old);
    actionMoveUp(idx, data,x, y, x_old, y_old);
    actionMoveDown(idx, data,x, y, x_old, y_old);
}

void Node::actionMoveUp(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old){

    x--;

    if(x>=0 && x!=x_old){
        vector<vector <int> > newData = data;   //make copy and swap stuff
        swap(newData[x+1][y],newData[x][y]);
        bool check = checkDuplicate(newData);
        if(check){
            totalNodesCreated++;
            nodesInfo[totalNodesCreated-1].addNode(nodesInfo[totalNodesCreated-1], newData, totalNodesCreated, idx);
            nodesInfo[totalNodesCreated-1].prevBlankPos = make_tuple(x+1,y);
            levelQ.push(totalNodesCreated);
        }
    }

}

void Node::actionMoveRight(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old){

    y++;

    if(y<3 && y!=y_old){

        vector<vector <int> > newData = data;   //make copy and swap stuff
        swap(newData[x][y-1],newData[x][y]);
        bool check = checkDuplicate(newData);
        if(check){
            totalNodesCreated++;
            nodesInfo[totalNodesCreated-1].addNode(nodesInfo[totalNodesCreated-1], newData, totalNodesCreated, idx);
            nodesInfo[totalNodesCreated-1].prevBlankPos = make_tuple(x,y-1);
            levelQ.push(totalNodesCreated);
        }
    }

}

void Node::actionMoveLeft(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old){

    y--;

    if(y>=0 && y!=y_old){

        vector<vector <int> > newData = data;   //make copy and swap stuff
        swap(newData[x][y+1],newData[x][y]);
        bool check = checkDuplicate(newData);
        if(check){

            totalNodesCreated++;
            nodesInfo[totalNodesCreated-1].addNode(nodesInfo[totalNodesCreated-1], newData, totalNodesCreated, idx);
            nodesInfo[totalNodesCreated-1].prevBlankPos = make_tuple(x,y+1);
            levelQ.push(totalNodesCreated);
        }
    }

}

void Node::actionMoveDown(int idx, vector<vector <int> > data, int x, int y, int x_old, int y_old){

    x++;

    if(x<3 && x!=x_old){

        vector<vector <int> > newData = data;   //make copy and swap stuff
        swap(newData[x-1][y],newData[x][y]);
        bool check = checkDuplicate(newData);
        if(check){
            totalNodesCreated++;
            nodesInfo[totalNodesCreated-1].addNode(nodesInfo[totalNodesCreated-1], newData, totalNodesCreated, idx);
            nodesInfo[totalNodesCreated-1].prevBlankPos = make_tuple(x-1,y);
            levelQ.push(totalNodesCreated);
        }
    }

}

void Node::addNode(Node& newNode, vector <vector <int> > data, int id, int pNum){
    newNode.block = data;
    newNode.idNum = id;
    newNode.parentNum = pNum;
    newNode.cost = nodesInfo[pNum-1].cost + 1;
    newNode.currBlankPos = newNode.posBlankTile(data);
}

tuple<int, int> Node::posBlankTile(vector <vector <int> > data){
    int i,j;
    for(i=0;i<3;++i){
        for(j=0;j<3;++j){
            if(data[i][j]==0){
                return make_tuple(i,j);
            }
        }
    }
    return make_tuple(-1,-1);
}

bool Node::checkDuplicate(vector<vector <int> > data){
    for(int i=0;i<totalNodesCreated;++i){
        if(data == nodesInfo[i].block)
        return false;
    }
    return true;
}

//Main function

int main()
{
    vector<vector<int> > data{{1,2,3},{4,5,6},{7,8,0}};
    totalNodesCreated++;
    nodesInfo[ROOT].addNode(nodesInfo[ROOT], data, 1, 0);
    tuple<int, int> prevBlankRoot(-1,-1);
    nodesInfo[ROOT].prevBlankPos = prevBlankRoot;
    nodesInfo[ROOT].cost = 0;

    levelQ.push(1);

    generateNodes();
    cout<<totalNodesCreated<<endl;
    cout<<nodesInfo[MAX-1].cost<<endl;

// Uncomment this block of code to find the data or values from a particular node, say Node 99,999

/*
    for(int i=0;i<3;++i){
        for(int j=0;j<3;++j){
            cout<<nodesInfo[99999].block[i][j]<<" ";
        }
        cout<<endl;
    }
*/

//Uncomment this part of code if you wish to see the data from all the nodes. Or set value of K for any particular range of nodes

    ofstream finalOut;
    finalOut.open("../output/output.txt");

    finalOut<<"Total nodes created  "<<totalNodesCreated<<"\n\n";

    for(int k=0; k<MAX;++k){
        finalOut<<"Node number "<<k+1<<"\nParent number "<<nodesInfo[k].parentNum<<"\nCost "<<nodesInfo[k].cost<<"\n\n";
        for(int i=0;i<3;++i){
            for(int j=0;j<3;++j){
                finalOut<<nodesInfo[k].block[i][j]<<" ";
            }
            finalOut<<"\n";
        }
        finalOut<<"\n";
    }

    finalOut.close();

    return 0;
}
