#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool edf(pair<int, int> t1, pair<int, int> t2) {
    return t1.second < t2.second;
}

void testcase() {
    int n; cin >> n; // # tasks (jedi)
    int m; cin >> m; // # time units (intervals)
    
    vector<pair<int, int> > tasks(n);
    vector<int> tasks_per_interval(m, 0);
    for (int i = 0 ; i < n ; i++) {
        int a; cin >> a; // Starting time
        int b; cin >> b; // Ending time
        
        tasks[i] = make_pair(--a, --b); // a, b are given from 1 to m (included)
        
        tasks_per_interval[a]++;
        tasks_per_interval[b]--;
        if (a > b) {
            tasks_per_interval[0]++;
        }
    }
    
    // First two testcases assumptions: there is an interval where there are 0 tasks
    int s = -1;
    int nb_tasks = 0;
    for (int i = 0 ; i < m ; i++) {
        nb_tasks += tasks_per_interval[i];
        if (nb_tasks == 0) {
            s = i;
            break;
        }
    }
    
    // Now that we have this unprotected segment, we can shift everything so that it starts there
    // that way, we ain't got no modular cases no more, and we can do basic EDF
    for (int i = 0 ; i < n ; i++) {
        int start = tasks[i].first;
        int deadline = tasks[i].second;
        
        if (start <= deadline) {
            // Normal case
            if (start > s) {
                // The task happens after the interval s - just shift it to the left
                tasks[i].first -= s;
                tasks[i].second -= s;
            } else {
                // The task happens before the interval s - just do m - this stuff
                tasks[i].first = m - (s - start);
                tasks[i].second = m - (s - deadline);
            }
        } else {
            // Modular case - we know that the start is after s and the end is before
            tasks[i].first -= s;
            tasks[i].second = m - (s - deadline);
        }
    }
    
    // Idea: schedule with EDF (Earliest Deadline First)
    random_shuffle(tasks.begin(), tasks.end());
    sort(tasks.begin(), tasks.end(), edf);
    
    int scheduled = 0;
    int finish = -1;
    int previous_finish = -1;
    for (auto it = tasks.begin() ; it != tasks.end() ; it++) {
        int start = it->first;
        int deadline = it->second;
        
        if (start > finish) {
            // We can simply schedule that task, since it doesn't conflict with any already scheduled tasks
            scheduled++;
            previous_finish = finish;
            finish = deadline;
        } else {
            // There is a conflict - but maybe this task ends sooner. If it does, we replace the previous one with this one
            if (deadline < finish) {
                finish = deadline;
            }
        }
    }
    
    cout << scheduled << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
