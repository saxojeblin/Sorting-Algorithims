#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

/* Monster structure and helper functions */

typedef struct monster {
	int id;
	char name[64];
	char element[64];
	int population;
	double weight;
} monster;

monster *make_some_monsters(int n)
{
  monster *monsters = malloc(sizeof(monster) * n);

  time_t t;

  srand((unsigned) time(&t));

  for(int i = 0; i < n; i++)
  {
    monsters[i].id = i;
    sprintf(monsters[i].name, "Monster #%d", rand());
    sprintf(monsters[i].element, "Element #%d", rand());
    monsters[i].population = rand();
    monsters[i].weight = 500.0 * ((double) rand() / (double) RAND_MAX);
  }

  return monsters;
}

void output_monster_list(monster *list, int n, char *title) {
  printf("List %s:\n", title);
  for(int i = 0; i < n; i++) {
    printf("  Monster %d: %s %s %d %lf\n", i, list[i].name, list[i].element, list[i].population, list[i].weight);
  }
  printf("\n");
}

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}

void swap_monsters(monster *list, int i, int j)
{
  monster temp;

  memcpy(&temp, list + i, sizeof(monster));
  memcpy(list + i, list + j, sizeof(monster));
  memcpy(list + j, &temp, sizeof(monster));
}

void check_monster_sort(monster *list, int n, int use_name, int use_weight)
{
  for(int i = 1; i < n; i++) {
    if(compare_monsters(list + i - 1, list + i, use_name, use_weight) > 0) {
      printf("*** The list is NOT sorted.\n\n");
      return;
    }
  }
  printf("The list is sorted.\n\n");
}

/* The core comparison function. */

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
  /* If we are comparing by name */
  if (use_name == 1)
  {
      /* If strcmp returns a positive, we are returning 1 */
      if(strcmp(m1->name,m2->name) > 0)
      {
          return 1;
      }
      /* If strcmp returns a negative, return 0 */
      else
      {
          return 0;
      }
  }
  /* If we are comparing by weight */
  else if (use_weight == 1)
  {
      if (m1->weight < m2->weight)
      {
          return 1;
      }
      else
      {
          return 0;
      }
  }
}

/* Implement ascending quick sort. */

int repartition(monster *list, int low_index, int high_index, int *comparisons, int *swaps,
                int use_name, int use_weight)
{
  monster *pivot_monster;
  pivot_monster = list + high_index;

  /* Have pivot index start at the low index, or 0 */
  int pivot_destination_index;
  pivot_destination_index = low_index;

  for (int i = low_index; i < high_index; i++)
  {
      /* If the new monster is greater than the first one, swap them and increase the pivot's index */
      (*comparisons)++;
      if (compare_monsters((list + i), pivot_monster, use_name, use_weight) == 0)
      {
        (*swaps)++;
        swap_monsters(list, pivot_destination_index, i);
        pivot_destination_index++;
      }
  }

  /* Finally, swap one last time and then return the pivot index */
  (*swaps)++;
  swap_monsters(list, pivot_destination_index, high_index);


  return pivot_destination_index;
}

/* Recursive function for quick sort. */

void quick_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *swaps,
                          int use_name, int use_weight)
{
    /* Find the pivot index with the repartition function */
  int pivot_index;
  pivot_index = repartition(list, low_index, high_index, comparisons, swaps, use_name, use_weight);

  /* Depending on where the pivot index is, call quick sort again */
  if (pivot_index - 1 > low_index)
  {
      quick_sort_recursive(list, low_index, pivot_index - 1, comparisons, swaps, use_name, use_weight);
  }

  if (high_index > pivot_index + 1)
  {
      quick_sort_recursive(list, pivot_index + 1, high_index, comparisons, swaps, use_name, use_weight);
  }
}

void quick_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();
  quick_sort_recursive(list, 0, n-1, &comparisons, &swaps, use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Implement ascending bubble sort. */

void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
  int i;
  int j;
  int temp;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Bubble sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();
  /* Traverse the list, with the number of indices decreasing by 1 every iteration */
  for (i = n-1; i>= 0; i--)
  {
      for (j=0; j<i; j++)
      {
          /* If the monster list[j + 1] is smaller than list[j], swap them */
          comparisons++;
          if (compare_monsters(list + j, list + j + 1, use_name, use_weight == 1))
          {
              swaps++;
              swap_monsters(list, j, j+1);
          }
      }
  }

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Highest-value finder for selection sort. */

int find_highest(monster *list, int n, int *comparisons, int use_name, int use_weight)
{
  monster *highest_monster = list;
  int highest_index = 0;
  /* Traverse the list */
  for (int i=1; i<= n; i++)
  {
    /* If the new monster list[i] is greater than highest_monster, swap them */
    (*comparisons)++;
    if (compare_monsters(list + i, highest_monster, use_name, use_weight) == 1)
    {
        highest_index = i;
        highest_monster = list + i;
    }
  }
  /* Return the index of the highest number in the list */
  return highest_index;
}

/* Implement ascending selection sort. */

void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  int i;
  int highest;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Selection sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  /* Traverse the list from the end to the beginning */
  for (i = n-1; i>0; i--)
  {
      /* Find the index of the highest monster in the list, and place it at the end */
      highest = find_highest(list, i, &comparisons, use_name, use_weight);
      if (highest != i)
      {
          swaps++;
          swap_monsters(list, highest, i);
      }
  }

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);

}

/* Find position for insertion sort. */

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int *comparisons, int use_name, int use_weight)
{
    int position;
    position = high_index - 1;

    /* While the position we are at is greater than 0 and the weight of list[position] is
    greater than k's weight */
    while (position >= low_index && compare_monsters(list + position, k, use_name, use_weight) == 1)
    {
        (*comparisons)++;
        --position;
    }

    /* The weight of k is greater than lis[position]'s weight */
    (*comparisons)++;
    ++position;

    return position;
}

/* Implement insertion sort. */

void insertion_sort_internal(monster *list, int n, int *comparisons, int *copies, int *block_copies, int use_name, int use_weight)
{
    int i, position;
    monster temp_monster;

    for (i = 1; i < n; i++)
        {
            /* Set a temporary monster equal to the monster of index 'i' in the list */
            temp_monster = list[i];
            (*copies)++;

            /* Find the position where the monster needs to be inserted */
            position = insertion_sort_find_position(list, 0, i, &temp_monster, comparisons, use_name, use_weight);

            if (position != i)
            {
                memmove(&list[position + 1], &list[position], sizeof(monster) * (i - position));
                list[position] = temp_monster;
                (*block_copies)++;
            }

            /* Insert the monster in the appropiate position in the list */
        }
}

void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int block_copies = 0;
  int total_copies = 0;
  clock_t start_cpu, end_cpu;
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  insertion_sort_internal(list, n, &comparisons, &total_copies, &block_copies, use_name, use_weight);

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d block copies (%d total copies).\n", comparisons, block_copies, total_copies);
  print_clocks(end_cpu - start_cpu);
  return;
}

void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2,
                      int *comparisons, int *copies, int *block_copies, int *mallocs,
                      int use_name, int use_weight)
{
    int i, j, k;
    int num1, num2;

    /* Num1 and Num2 are the sizes of the temporary arrays made below */
    num1 = h1 - l1 + 1;
    num2 = h2 - h1;

    /* Temporary arrays to store the values of the two sub arrays passed in */
    monster *temp1;
    monster *temp2;

    temp1 = malloc(sizeof(monster) * num1);
    (*mallocs)++;

    temp2 = malloc(sizeof(monster) * num2);
    (*mallocs)++;

    /* Populate the arrays with the data from the list */
    for (i=0; i<num1; i++)
    {
        temp1[i] = list[l1 + i];
    }

    (*block_copies)++;
    for (j=0; j<num2; j++)
    {
        temp2[j] = list[l2 + j];
    }
    (*block_copies)++;

    /* Initialize these variables to be the starting indices of the sub arrays and list */
    i = 0, j = 0, k = l1;

    /* Compare the two halves with one another and whichever value is less at each index,
       put that value at the front of the actual list */
    while (i < num1 && j < num2)
    {
        if (compare_monsters(&temp1[i], &temp2[j], use_name, use_weight) == 0)
        {
            list[k] = temp1[i];
            i++;
        }
        else
        {
            list[k] = temp2[j];
            j++;
        }
        (*copies)++;
        (*comparisons)++;
        k++;
    }

    /* Copy any remaining elements of both the subarrays into the list*/
    while (i < num1)
    {
        (*copies)++;
        list[k] = temp1[i];
        i++;
        k++;
    }
    while (j < num2)
    {
        (*copies)++;
        list[k] = temp2[j];
        j++;
        k++;
    }
}

/* Recursive function for merge sort. */

void merge_sort_recursive(monster *list, int low_index, int high_index,
                          int *comparisons, int *copies, int *block_copies, int *mallocs,
                          int use_name, int use_weight)
{
    if (low_index < high_index)
    {
        /* Find the middle index of the list */
        int middle = low_index + (high_index - low_index) / 2;

        /* Sort the two seperate halves */
        merge_sort_recursive(list, low_index, middle, comparisons, copies, block_copies, mallocs, use_name, use_weight);
        merge_sort_recursive(list, middle + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);

        /* Merge the two halves together */
        merge_sort_merge(list, low_index, middle, middle +1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    }
}

/* Implement merge sort. */

void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge sort %d monsters...\n", n);

  start_cpu = clock();
  merge_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                       use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);

}

/* Recursive function for merge-insertion sort. */

void merge_insertion_sort_recursive(monster *list, int low_index, int high_index,
                                    int *comparisons, int *copies, int *block_copies, int *mallocs,
                                    int use_name, int use_weight)
{
    /* Merge sort until the amount of indices in the list is equal to or less than 25 */
    if (high_index - low_index >= 25)
    {
        /* Find the middle index of the list */
        int middle = low_index + (high_index - low_index) / 2;

        /* Sort the two seperate halves */
        merge_insertion_sort_recursive(list, low_index, middle, comparisons, copies, block_copies, mallocs, use_name, use_weight);
        merge_insertion_sort_recursive(list, middle + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);

        merge_sort_merge(list, low_index, middle, middle +1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    }

    /* Our list passed in has n<=25, so now we insertion sort that list */
    else
    {
        insertion_sort_internal(list + low_index, high_index - low_index + 1, comparisons, copies, block_copies, use_name, use_weight);
    }
}

/* Implement merge sort. */

void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge-insertion sort %d monsters...\n", n);

  start_cpu = clock();
  merge_insertion_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                                 use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);
}

/* Main program. */

void run_all_sorts(int n, int only_fast, int use_name, int use_weight) {
  monster *our_list = make_some_monsters(n);
  monster *our_unsorted_list = malloc(sizeof(monster) * n);

  printf("SORT SET: n = %d, %s, by %s\n\n", n, only_fast ? "fast sorts only" : "all sorts", use_name ? "name" : "weight");

  if(only_fast == 0) {
    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    bubble_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    selection_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    insertion_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);
  }

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  quick_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_insertion_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  printf("SORT SET COMPLETE\n\n");

  free(our_list);
  free(our_unsorted_list);
}

int main(void) {
  run_all_sorts(50, 0, 0, 1);
  run_all_sorts(50, 0, 1, 0);
  run_all_sorts(500, 0, 0, 1);
  run_all_sorts(500, 0, 1, 0);
  run_all_sorts(5000, 0, 0, 1);
  run_all_sorts(5000, 0, 1, 0);
  run_all_sorts(50000, 1, 0, 1);
  run_all_sorts(50000, 1, 1, 0);
  run_all_sorts(500000, 1, 0, 1);
  run_all_sorts(500000, 1, 1, 0);

}
