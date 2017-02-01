#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <float.h>

using namespace std;

struct dot{
  int x;
  int y;
}input[100000];

struct {
  struct dot from;
  struct dot to;
}result[50000];

int resultidx = 0;
float minium = FLT_MAX;

float dist(struct dot a, struct dot b){
  return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

int compare(const void *a, const void *b){
  struct dot *x = (struct dot*)a;
  struct dot *y = (struct dot*)b;
  if(x->x != y->x)
    return (x->x - y->x);
  else
    return (x->y - y->y);
}

bool exist(struct dot a, struct dot b){
  for(int i=0 ; i<resultidx ; i++){
    if(result[i].from.x == a.x && result[i].from.y == a.y && result[i].to.x == b.x && result[i].to.y == b.y)
      return true;
    else if(result[i].from.x == b.x && result[i].from.y == b.y && result[i].to.x == a.x && result[i].to.y == a.y)
      return true;
  }
  return false;
}

float bruteForce(struct dot P[], int n){
  float min = FLT_MAX;
  for(int i=0 ; i<n-1 ; i++){
    for(int j=i+1 ; j<n ; j++){
      if(dist(P[i], P[j]) < min){
        min = dist(P[i], P[j]);
      }
      if(dist(P[i], P[j]) < minium){
        minium = dist(P[i], P[j]);
        memset(result, 0, sizeof(result));
        resultidx = 0;
        result[resultidx].from = P[i];
        result[resultidx].to = P[j];
        resultidx++;
      }
      else if(dist(P[i], P[j]) == minium){
        if(exist(P[i], P[j])) continue;
        result[resultidx].from = P[i];
        result[resultidx].to = P[j];
        resultidx++;
      }
    }
  }
  return min;
}

float InRange(struct dot P[], int size, float min){
  for(int i=0 ; i<size-1 ; i++){
    for(int j=i+1 ; j<size && (P[j].y - P[i].y) <= min ; j++){
      if(dist(P[i], P[j]) < min){
        min = dist(P[i], P[j]);
      }
      if(dist(P[i], P[j]) < minium){
        minium = dist(P[i], P[j]);
        memset(result, 0, sizeof(result));
        resultidx = 0;
        result[resultidx].from = P[i];
        result[resultidx].to = P[j];
        resultidx++;
      }
      else if(dist(P[i], P[j]) == minium){
        if(exist(P[i], P[j])) continue;
        result[resultidx].from = P[i];
        result[resultidx].to = P[j];
        resultidx++;
      }
    }
  }
  return min;
}

float dnc(struct dot P[], int n){
  qsort(P, n, sizeof(struct dot), compare);
  /*for(int i=0 ; i<size ; i++){
    cout << P[i].x << " " << P[i].y << endl;
  }*/
  if(n <= 3)
    return bruteForce(P, n);
  int mid = n/2;
  struct dot midP = P[mid];
  float left = dnc(P, mid);
  float right = dnc(P+mid, n-mid);
  float d = min(left, right);

  struct dot point[n];
  int k = 0;
  for(int i=0 ; i<n ; i++){
    if(abs(P[i].x - midP.x) < d){
      point[k] = P[i];
      k++;
    }
  }
  return min(d, InRange(point, k, d));
}

int main(int argc, char const *argv[]) {
  fstream file;
  fstream out;
  char buf[1024];
  int idx = 0;

  file.open(argv[1], ios::in);
  if(!file){
    cerr << "open file failed." << endl;
    return -1;
  }
  while(!file.eof()){
    file.getline(buf, sizeof(buf));
    input[idx].x = (int)buf[0] - 48;
    input[idx].y = (int)buf[2] - 48;
    idx++;
  }
  idx--;
  file.close();

  out.open("output_divideandconquer.txt", ios::out);
  out << dnc(input, idx) << endl;
  for(int i=0 ; i<resultidx ; i++){
    out << result[i].from.x << " " << result[i].from.y << " " << result[i].to.x << " " << result[i].to.y << endl;
  }
  out.close();

  return 0;
}
