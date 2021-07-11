#pragma GCC optimize "trapv"
#include <iostream>
#include<bits/stdc++.h>

#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
#include<functional>
using namespace std;
using namespace chrono;
typedef vector< int > vi;
typedef vector< vi > vvi;
typedef vector< long long > vll;
typedef pair< int,int > ii;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;

#define all(c) c.begin(),c.end()


// struct for storing a transaction 
struct transaction{
    string txid;
    long long fee;
    long long weight;
    vector<string> parents;
};

vector<transaction> transactions; // vector which stores transactions

// mapping between transaction ids and a boolean which is true if the transaction is 
// included in the block and false otherwise
unordered_map<string,bool> is_included;

// a mapping between transaction id and number of transactions it is a parent
unordered_map<string,ll> frequency_of_being_parent;

unordered_map<string,set<string>> x;
unordered_map<string , ll > indegree;
unordered_map<string ,string > max_child;
unordered_map<string , ll > fees;
unordered_map<string , ll > weight;
unordered_map<string , ld > priori;

// returns true if a transaction has been included in the block false

bool isExecuted(transaction t){
    if(is_included[t.txid]=true){
        return true;
    }
    return false;
}
 
// custom comparator based on priority for sorting the vector
bool compare(transaction a, transaction b){
    ld priority_of_a =  (ld)(a.fee * (frequency_of_being_parent[a.txid]+1))/(ld)(a.weight* (a.parents.size()+1));
    ld priority_of_b = (ld)(b.fee *(frequency_of_being_parent[b.txid]+1))/(ld)(b.weight * (b.parents.size()+1));

  /*   if(x[a.txid].find(b.txid) != x[a.txid].end()){
        return 1;
    }
    if(x[b.txid].find(a.txid) != x[b.txid].end()){
        return 0;
    } */
    return priority_of_a > priority_of_b ; // if a has higher priority it comes in front after sorting with this comparator 
}


int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    #ifndef ONLINE_JUDGE 
    freopen("input.txt", "r", stdin); 
    freopen("output.txt", "w", stdout); 
    freopen("error.txt","w",stderr);
    #endif
    	auto start1 = high_resolution_clock::now();
	

    // taking input begins 
    ifstream inputFile("mempool.csv");
    vector<string> row;
    string word;
    
    
    while (inputFile) {
        row.clear();
        string s;
        if (!getline(inputFile, s)) break;
      
        if (s[0] != 't') {   // used for filtering the first line of csv which has column names
            
            istringstream ss(s); // convert the line into string stream
            transaction tem;     // declaring a new transaction instance
       
            while (ss) {
                string line;
                if (!getline(ss, line, ','))   // take values from the stringstream separeted by commas
                    break;
                try {
                   row.push_back(line);    // insert them into a vector of strings
                }
                catch (const std::invalid_argument e) {
                   
                }
            }

            tem.txid = row[0];                  // assigning values to the transaction 
            tem.fee = stoi(row[1]);
            tem.weight  = stoi(row[2]);
             fees[tem.txid] = tem.fee;
              weight[tem.txid] = tem.weight;
              priori[tem.txid] = (ld(tem.fee))/tem.weight;
            if(row.size()>3){     // if size of row is greater than 3 which means this transaction has a parent list

                    stringstream parents_stream(row[3]);
                    vector<string> par; 
                    while(getline(parents_stream, word, ';')) { // separating the parents with ';' 
                        par.push_back(word);  //inserting them into a temporary vector
                    }
                    
                    tem.parents = par;   // assigning this vector to parents attribute of the transaction
            }
           
            transactions.push_back(tem);                   
                    
 
        }
    }
    // input has been taken and transactions are stored in a vector



    // **some preprocessing begins**
    // most importantly first we sort the transactions based on a custom compare function which decides the priority for sorting
    // then we
    // will greedily choose transactions keeping in mind the constraints posed due to existence of parents
    sort(all(transactions),compare);

    cout<<transactions.size()<<endl;
    long long sum_of_reward_of_all_transactions = 0;
    for(auto  transaction : transactions){

        sum_of_reward_of_all_transactions+=transaction.fee; // calculating sum of fee of all transactions just for a metric 

        for(auto parent : transaction.parents){
            if(max_child.find(parent) == max_child.end()){
                max_child[parent] = transaction.txid;
            }
            else{
                if(transaction.fee > fees[max_child[parent]]){
                     max_child[parent] = transaction.txid;
                }
            }
            indegree[transaction.txid]++;
            x[parent].insert(transaction.txid);
            frequency_of_being_parent[parent]++;   // whenever a transaction appears as parent its counter is increased by one
        }
    }
    // preprocessing ends

    
    /*  Actual code begins */


    ll capacity = 4000000; // max capacity for block 4 million 
    ll profit=0;
    ld factor = 1;
    priority_queue<pair<ld,string>> queue;
    for(auto i : transactions){
        if(indegree[i.txid]==0ll){
            int c=1;
            ld p=0;
            ll w=1;
            int c1=1;
            ld sum=priori[i.txid];
            for(auto j : x[i.txid]){
                p = max(p,priori[j]);
                sum+= priori[j];
                c=2;
                c1++;
            }
            c=1;
            if(p < factor*priori[i.txid]){ // this is where i say that if max priori among all children is less than priori of the parent just dont include the effect of children while inserting in priority queue
                p = ((ld)i.fee/i.weight);
               
            }
            //queue.push(make_pair((((ld)i.fee/i.weight)+p)/((ld)c),i.txid));
             queue.push(make_pair((p)/((ld)c),i.txid));
        }
    }
    vector<string> ans;

    while (capacity > 0 && !queue.empty())
    {
        auto i = queue.top();
        queue.pop();
        if(capacity< weight[i.second]){
            continue;
        }
        profit+= fees[i.second];
        capacity -= weight[i.second];
        ans.push_back(i.second);

        for(auto k : x[i.second]){
                indegree[k]--;
                if(indegree[k]==0){
                   int c=1;
                    ld p=0;
                    ll w=1;
                    int c1=0;
                    ld sum = priori[k];
                    for(auto j : x[k]){
                        p = max(p,priori[j]);
                        sum+= priori[j]; 
                        c=2;
                        c1++;
                    }
                    c=1;
                     if(p < factor*priori[k]){
                        p= priori[k];
                        
                    }
                    //queue.push(make_pair((((ld)fees[k]/weight[k])+p)/((ld)c),k));
                    queue.push(make_pair((p)/((ld)c),k));
                }
        }


    }

    cout<<"sum_of_reward_of_all_transactions            | " <<sum_of_reward_of_all_transactions<<endl; 
    
    cout<<"\nnumber of transactions included              | "<<ans.size()<<endl; // number of transactions included

    cout<<"\nTotal sum of reward of included transactions | "<<profit<<endl;
    set<string> s;
    cout<<"\ncapacity unutilized                          | "<<capacity<<endl;
    cout<<"\nincluded Transactions\n"<<endl;
     for(auto i : ans){
         cout<<i<<endl;
     }

    


     auto stop1 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop1 - start1);
    #ifndef ONLINE_JUDGE
        cerr << "Time: " << duration . count() / 1000 << endl;
    #endif
    //cout<<s.size();

    


    


    

    
 


   return 0;
}

