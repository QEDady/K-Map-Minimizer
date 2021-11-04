#include <bits/stdc++.h>
using namespace std;

const int MinVaribles = 2;
const int MaxVaribles = 4;

//Takes the input and validates it
void takeInput(int & numVaribales, int &numMinTerms, set<int> &minTerms){
    //numVariables with validation
    cout<<"Enter the number of variables you want between "<< MinVaribles<<" and "<<MaxVaribles<<" variables\n";
    cin>>numVaribales;
    while ( numVaribales<MinVaribles || numVaribales>MaxVaribles)
    {
        cout<<"You entered a number out of range.\n";
        cout<<"Enter the number of varibles you want between "<< MinVaribles<<" and "<<MaxVaribles<<" variables\n";
        cin>>numVaribales;
    }
    //numMinTerms with validation
    cout<<"Enter the number of minterms you want to provide.\n";
    cin>>numMinTerms;

    while(numMinTerms <0 || numMinTerms> 1<<numVaribales){
        cout<<"You entered a number out of range.\n";
        cout<<"Enter a non-negative number less than or equal "<<1<<numVaribales<<"\n";
        cin>>numMinTerms;
    }

    //Getting Minterms with validation
    for(int i=1;i<=numMinTerms;i++){
        int cur;
        cout<<"Enter the minterm number "<<i<<"\n";
        cin>>cur;

        while(cur<0 || cur>= 1<<numVaribales){
            cout<<"Your minterm should be between "<<0<<" and "<<(1<<numVaribales)-1<<" inclusive\n";
            cout<<"Enter the minterm number "<<i<<"\n";
            cin>>cur;
        }

        while(minTerms.count(cur)){
            cout<<"You have entered that minterm before\n";
            cout<<"Enter the minterm number "<<i<<"\n";
            cin>>cur;
          while(cur<0 || cur>= 1<<numMinTerms){
            cout<<"Your minterm should be between "<<0<<" and "<<(1<<numMinTerms)-1<<" inclusive\n";
            cout<<"Enter the minterm number "<<i<<"\n";
            cin>>cur;
        }
        }
        minTerms.insert(cur);
    }
}

void printKmap(int numVariables, set<int> minTerms){
    cout<<"The Kmap is :\n";
    if(numVariables==2){
        cout<<"B\\A|";
    }
    else if(numVariables==3){
        cout<<"C\\AB|";
    }
    else if(numVariables==4){
        cout<<"CD\\AB|";
    }
    map<int, pair<vector<string>, vector<string>>> mp;//maps the num of vars to rows and cols in the kmap

    mp[2]={{"0", "1"}, {"0", "1"}};

    mp[3]= {{"00", "01", "11", "10"}, {"0", "1"}};
    mp[4]= {{"00", "01", "11", "10"}, {"00", "01", "11", "10"}};

    vector<string> cols= mp[numVariables].first;
    vector<string> rows = mp[numVariables].second;

    for(int i=0; i<cols.size();i++)
        cout<<cols[i]<<"|";
    cout<<"\n";
    for(int row=0; row<rows.size();row++){
        cout<<rows[row];
        if(numVariables==2)
            cout<<"  |";
        else
            cout<<"   |";

        for(int col=0;col<cols.size();col++){
            int num= stoi(cols[col]+rows[row], 0, 2);
            if(minTerms.count(num))
                cout<<1;
            else
                cout<<0;
        if(numVariables==2)
            cout<<"|";
        else
            cout<<" |";

        }
        cout<<"\n";
    }
    cout<<"------------------------------------------------\n";
}

//get first bits of binary representation of num
//wit the number of setBits
pair<int, string> getBinaryRep(int num, int bits){
    string s="";
    int setBits=0;
    for(int i=0;i<bits;i++){
        if(num>>i & 1){
            s.push_back('1');
            setBits++;
        }
        else
            s.push_back('0');
    }
    reverse(s.begin(), s.end());
    return {setBits, s};
}

bool simplify(string x, string y, string &res){
    int n=x.size();
    int dif=0;
    for(int i=0;i<n;i++){
        if(x[i]!=y[i]){
            dif++;
            res.push_back('-');
        }
        else
            res.push_back(x[i]);
    }

    return dif==1;
}

vector<string> calcPrimeImplicants(int numVariables, set<int> minTerms ){
    //minTermsWithBits.first: number of set bits in the minterm
    vector<pair<int, string>> minTermsWithBits;
    for(int term:minTerms){
        minTermsWithBits.push_back(getBinaryRep(term, numVariables));
    }
    sort(minTermsWithBits.begin(), minTermsWithBits.end());
    vector<set<string>> successiveGroups;

    set<string> st;
    if(!minTermsWithBits.empty()) st.insert(minTermsWithBits[0].second);
    for(int i=1;i<minTermsWithBits.size();i++){
        if(minTermsWithBits[i].first!=minTermsWithBits[i-1].first){
            successiveGroups.push_back(st);
            st.clear();
        }
        st.insert(minTermsWithBits[i].second);
    }
    if(!st.empty()){
        successiveGroups.push_back(st);
        st.clear();
    }
    set<string> simplified;
    vector<string> primeImplicants;

    vector<set<string>> tmpSuccessiveGroups;
    while(successiveGroups.size()) {
        for (int i = 1; i < successiveGroups.size(); i++) {
            set<string> first = successiveGroups[i - 1], second = successiveGroups[i];
            for (string x:first) {
                for (string y: second) {
                    string res;
                    if (simplify(x, y, res)) {
                        st.insert(res);
                        simplified.insert(x);
                        simplified.insert(y);
                    }
                }
                if (!simplified.count(x))
                    primeImplicants.push_back(x);
            }
            if (!st.empty()) {
                tmpSuccessiveGroups.push_back(st);
                st.clear();
            }
        }

        for(string x: successiveGroups.back()){
            if(!simplified.count(x))
                primeImplicants.push_back(x);
        }
        successiveGroups.clear();
        successiveGroups=tmpSuccessiveGroups;
        tmpSuccessiveGroups.clear();
    }
    return primeImplicants;
}

bool primeeContainsTerm(string primeImp, int term){
    int k=primeImp.size();
    for(int i=0;i<k;i++){
        if(
                ((term>>(k-i-1)&1) && primeImp[i]=='0')
                ||
                        (!(term>>(k-i-1)&1) && primeImp[i]=='1')
        )
            return false;

    }
    return true;
}
void simplifyPrimesAndAnswer(int numVariables, vector<string> primeImplicants, set<int> minTerms){
    map<string, set<int>> primeToMinTerms;
    int numMinTerms= minTerms.size();
    for(string prime:primeImplicants){
        set<int> st;
        for(int term:minTerms){
            if(primeeContainsTerm(prime, term))
                st.insert(term);
        }
        primeToMinTerms[prime] = st;
    }
    vector<string> chosenPrimes;

    int coveredMinTerms=0;
    for(int term: minTerms){
        int cnt=0;
        string essentialPrime;
        for(auto it:primeToMinTerms){
            if(it.second.count(term)){
                cnt++;
                essentialPrime=it.first;
            }

           if(cnt>1)
                break;
        }

        if(cnt==1){
            chosenPrimes.push_back(essentialPrime);//this prime is essential
            for(int term:primeToMinTerms[essentialPrime]){
                coveredMinTerms++;
                for(auto &it:primeToMinTerms){
                    if(it.first==essentialPrime) continue;
                    if(it.second.count(term)) it.second.erase(term);
                }
            }
            primeToMinTerms.erase(essentialPrime);
        }

    }

    int remMainterms = numMinTerms-coveredMinTerms;
    vector<bool> choose(primeToMinTerms.size(), 0);
    for(int numSelected=0; numSelected<=primeToMinTerms.size();numSelected++){
        fill(choose.begin(), choose.end(), 0);
        fill(choose.end() - numSelected, choose.end(), 1);
        set<int> coveredTerms;
        do{
            int ch=0;
           coveredTerms.clear();
            for(auto it:primeToMinTerms){
                if(choose[ch]){
                    coveredTerms.insert(it.second.begin(), it.second.end());
                }
                ch++;
            }
            if(coveredTerms.size()==remMainterms)
                break;
        } while (next_permutation(choose.begin(), choose.end()));

        if(coveredTerms.size()==remMainterms){
            ///chosenPrimes
            int ch=0;
            for(auto it: primeToMinTerms){
                if(choose[ch]){
                    chosenPrimes.push_back(it.first);
                }
                ch++;
            }
            break;
        }

    }

    cout<<"The simplified function is:\n";
    cout<<"F = ";

    if(chosenPrimes.empty()){
        cout<<"0\n";
    }
    else {
        string s="";
        for(int j=0;j<chosenPrimes.size();j++){
            string prime=chosenPrimes[j];
            for(int i=0;i<prime.size();i++){
                if(prime[i]=='1')
                    s.push_back('A'+i);
                else if(prime[i]=='0'){
                    s.push_back('A'+i);
                    s.push_back('\'');
                }
            }
            if(j!=chosenPrimes.size()-1)
                s.append(" + ");
        }
        if(s.empty())
            cout<<"1\n";
        else
            cout<<s<<"\n";
    }
}

int main() {
    int numVaribales, numMinTerms;
    set<int> minTerms;
    takeInput(numVaribales, numMinTerms, minTerms);
    printKmap(numVaribales, minTerms);
    vector<string> primeImplicants;
    primeImplicants = calcPrimeImplicants(numVaribales, minTerms);
    simplifyPrimesAndAnswer(numVaribales, primeImplicants, minTerms);

    system("pause");
return 0;
}