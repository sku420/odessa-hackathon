
#include<bits/stdc++.h>
using namespace std;

struct income_sheet
{
	string date;
	float income_per_rover[100];
	float total;
};

class chargingPoints
{
public:
	pair<int,int>cpPos;
    int cp_id;
    int no_of_rovers;
	chargingPoints(pair<int,int> cpPos,int cp_id,int no_of_rovers)
	{
		this->cpPos = cpPos;
		this->cp_id = cp_id;
		this->no_of_rovers = no_of_rovers;
	}
};

void getMin(vector<vector<pair<int,int> > >&city, chargingPoints cp[],int i, int j, int k, int currDis)
{
    int dis = abs(i-cp[k].cpPos.first)+abs(j-cp[k].cpPos.second);
    if(dis < currDis)
    {
        city[i][j].first = dis;
        city[i][j].second = cp[k].cp_id;
    }
}

void chargeSheet(vector<vector<pair<int,int> > >&city,int m,int n,chargingPoints cp[])
{
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            for(int k=0;k<5;k++)
                getMin(city,cp,i,j,k,city[i][j].first);
}
int distnc[100]={0};

class rover
{
public:
    int batary;
    pair<int,int> pos;
    int rid;
    int cpid;
    int distance_travelled;

    rover(pair<int,int> pp, int r1id, int cp1id)
    {
        batary = 100;
        pos = pp;
        rid = r1id;
        cpid = cp1id;
        distance_travelled = 0;
    }
    int man_distance(pair<int,int> c1,pair<int,int> c2)
    {
        int x1 = abs(c1.first - c2.first);
        int x2 = abs(c1.second - c2.second);
        return x1 + x2;
    }
    void updateit(pair<int,int>dest)
    {
        int travelling_now = man_distance(this->pos,dest);
        this->distance_travelled += (travelling_now*10);
        distnc[this->rid]+=(travelling_now*10);
        int battary_consumed = ceil(travelling_now / 100);
        this->batary -= battary_consumed;
        this->pos = dest;
    }    
};

void rover_selection(vector<vector<pair<int,int> > > &city,vector<rover>&rovers,
	pair<int,int>src,pair<int,int>desti,chargingPoints cp[])
{
	int distance = INT_MAX;
	rover usinn = rover({0,0},0,0);
	for(auto i : rovers)
	{
        int available_battary = i.batary;
        int can_travel = available_battary*100;
        int has_to_travel = i.man_distance(i.pos,src) + i.man_distance(src,desti) + city[desti.first][desti.second].first;
        if(has_to_travel > can_travel)
                continue;
        else
        {
            if(distance > i.man_distance(i.pos,src) + i.man_distance(src,desti))
            {
                distance = i.man_distance(i.pos,src) + i.man_distance(src,desti);
                usinn = i;
            }
        }
	}

	if(distance == INT_MAX)
	    cout << "No rover available...!!"<< endl;
	else
	{
	    usinn.updateit(src);
        usinn.updateit(desti);
        cout<<usinn.distance_travelled<<endl;
        if(usinn.batary <= 30)
   		{
           pair<int,int> pp=city[desti.first][desti.second];
           int d = pp.first, d1 = pp.second;
           cp[d1].no_of_rovers += 1;
           usinn.batary = 100;
           usinn.updateit(cp[d1].cpPos);
   		}
	}
}

float getIncome(string from,string to, vector<income_sheet>&income)
{
	float total_income=0;
	bool flag=false;
	for(int i=0;i<income.size();i++)
	{
		if(income[i].date==from)
		{
			while(income[i].date<=to and i<income.size())
			{
				total_income+=income[i].total;
				i++;
			}
			flag=true;
		}
		if(flag)
			break;
	}
	return total_income;
}

void updateIncome(vector<income_sheet>&income,vector<rover>&rovers,string date,int no_of_rovers)
{
	income_sheet ob;
	ob.date=date;
	float total=0;
	for(int i=0;i<no_of_rovers;i++)
	{
		if(distnc[i]<1000)
			total+=(((float)distnc[i]/100)*0.50);
		else if(1000<=distnc[i]<2000)
			total+=(((float)distnc[i]/100)*0.75);
		else if(distnc[i]>=2000)
			total+=(((float)distnc[i]/100)*0.85);
	}
	ob.total=total;
	income.push_back(ob);
}

int main(int argc, char const *argv[])
{
	cout<<"\t**************Welcome**************"<<endl;

	vector<income_sheet>income;
    vector<vector<pair<int,int>> > city(50,vector<pair<int,int> >(50,{INT_MAX,0}));
    int no_of_rovers;
    cout << "Enter number of rovers\n";
    cin >> no_of_rovers;

    int no_of_rovers_on_one = no_of_rovers/5;

    chargingPoints cp[5] = {chargingPoints({0,0},1,no_of_rovers_on_one),
    						chargingPoints({0,49},2,no_of_rovers_on_one),
    						chargingPoints({49,0},3,no_of_rovers_on_one),
    						chargingPoints({49,49},4,no_of_rovers_on_one),
    						chargingPoints({24,24},5,no_of_rovers-(no_of_rovers_on_one*4))};
    vector<rover> rovers;
    for(int i=0;i<4;i++)
    {
    	for(int j=0;j<no_of_rovers_on_one;j++)
    	{
    		rovers.push_back( rover(cp[i].cpPos,i*no_of_rovers_on_one+j,cp[i].cp_id));
    	}
    }
    for(int i=0;i<(no_of_rovers-(no_of_rovers_on_one*4));i++)
    	rovers.push_back( rover(cp[4].cpPos,4*no_of_rovers_on_one+i,cp[4].cp_id));

    chargeSheet(city,50,50,cp);
    
    string date;
    while(true)
    {
    	int n;
    	cout << "Enter your choices\n";
    	cout << "1. For order\n";
    	cout << "2. For get income\n";
    	cout << "3. Next Date\n";
    	cout << "4. For exit...\n";
    	cin >> n;
    	switch(n)
    	{
    		case 1:
	    		{	A:
	    			int i1,j1,i2,j2;
	    			cin >> date;
	    			cin >> i1 >> j1 >> i2 >> j2;
	    			rover_selection(city,rovers,{i1,j1},{i2,j2},cp);
	    			updateIncome(income,rovers,date,no_of_rovers);
    			}
    			break;
    		case 2:
	    		{	string from,to;
	    			cin>>from>>to;
	    			cout << getIncome(from,to,income) << endl;
	    		}
    			break;
    		case 3:
    			{
    				updateIncome(income,rovers,date,no_of_rovers);
    				goto A;
    			}
    				break;
    		case 4:
    			cout << " Byeeee!!!!";
    			exit(0);
    	}
    }
    return 0;
}
