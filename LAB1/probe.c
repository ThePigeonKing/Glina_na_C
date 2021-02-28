#include "stdio.h"

int in_array(double num, double *p, int tally){
  for (int i = 0; i < tally; i++){
    if (*(p + i) == num)
      return 1;
  }
  return 0;
}

int main(){
  double a[5];
  double x;
  printf("Enter elements\n");
  for (int i = 0; i < 5; i++)
    scanf("%lf", &a[i]);
  scanf("%lf", &x);
  if (in_array(x, a, 5) == 1)
    printf("YES\n");
  else
    printf("NO\n");
}
