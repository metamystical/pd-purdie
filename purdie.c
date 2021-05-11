#include "m_pd.h"
#include <stdlib.h>
#include <time.h>

t_class *purdie_class;

typedef struct purdie_node {
  struct purdie_node *left;
  struct purdie_node *right;
  int num;
  int mute;
};

typedef struct purdie {
  t_object x_obj;
  t_inlet *in_lower, *in_upper, *in_fraction, *in_extra;
  t_outlet *out_series;
  int lower;
  int upper;
  float fraction;
  int fracint; // floor of fraction (int)
  int array_size;
  int *array;
  int index;
  struct purdie_node *root;
} t_purdie;

struct purdie_node *purdie_addNode (struct purdie_node *p, int n) {
  if (p == NULL) {
    p = (struct purdie_node *)getbytes(sizeof(struct purdie_node));
    p->num = n;
    p->mute = 0;
    p->left = NULL;
    p->right = NULL;
  }
  else if (n < p->num) p->left = purdie_addNode(p->left, n);
  else if (n > p->num) p->right = purdie_addNode(p->right, n);
  else if (n == p->num) p->mute = !p->mute;
  return p;
}

int purdie_getTreeSize (struct purdie_node *p) {
  if (p == NULL) return 0;
  return purdie_getTreeSize(p->left) + purdie_getTreeSize(p->right) + !p->mute;
}

void purdie_fillArray (struct purdie_node *p, t_purdie *x) {
  if (p == NULL) return;
  purdie_fillArray(p->left, x);
  if (!p->mute) x->array[x->index++] = p->num;
  purdie_fillArray(p->right, x);
}

void purdie_freeTree (struct purdie_node *p) {
  if (p == NULL) return;
  purdie_freeTree(p->left);
  purdie_freeTree(p->right);
  freebytes(p, sizeof(struct purdie_node));
}

int purdie_randomInt (int a, int b) { // returns random int in interval [a, b]
  double random = (double)rand() / (double)((unsigned)RAND_MAX + 1); // [0.0, 1.0)
  return (int)(random * (b - a + 1)) + a;
}

void purdie_shuffleArray(t_purdie *x, int begin, int end) { // Fisher–Yates shuffle algorithm
  for (int i = end; i > begin; i--) { // shuffle
    int j = purdie_randomInt(begin, i);
    int tmp = x->array[i];
    x->array[i] = x->array[j];
    x->array[j] = tmp;
  }
}

void purdie_reset (t_purdie *x) {
  if (x->upper < x->lower) {
    error("[purdie ]: upper < lower - swapping");
    int tmp = x->upper;
    x->upper = x->lower;
    x->lower = tmp;
  }
  if (x->fraction > 0.5) {
    error("[purdie ]: fraction > 0.5 - setting to 0.5");
    x->fraction = 0.5;
  }
  else if (x->fraction < 0) {
    error("[purdie ]: fraction < 0 - setting to 0");
    x->fraction = 0;
  }
  if (x->root == NULL) {
    for (int i = 0; i < x->upper - x->lower + 1; i++) x->root = purdie_addNode(x->root, x->lower + i);
  }
  x->array_size = purdie_getTreeSize(x->root);
  x->array = (int *)getbytes(x->array_size * sizeof(int));
  x->index = 0;
  purdie_fillArray(x->root, x);
  x->index = 0;
  x->fracint = (int)(x->fraction * x->array_size);
  purdie_shuffleArray(x, 0, x->array_size - 1); // full shuffle
}

void purdie_bang (t_purdie *x) {
  if (x->index == x->array_size) {
    if(x->fracint) purdie_shuffleArray(x, 0, x->array_size - x->fracint - 1); // omit last fraction
    purdie_shuffleArray(x, x->fracint, x->array_size - 1); // omit first fraction
    x->index = 0;
  }
  outlet_float(x->out_series, (float)x->array[x->index++]);
}

void purdie_freeReset (t_purdie *x) {
  freebytes(x->array, x->array_size);
  purdie_freeTree(x->root);
  x->root = NULL;
  purdie_reset(x);
}
  
void purdie_lower (t_purdie *x, t_floatarg f) {
  x->lower = (int)f;
  purdie_freeReset(x);
}

void purdie_upper (t_purdie *x, t_floatarg f) {
  x->upper = (int)f;
  purdie_freeReset(x);
}

void purdie_fraction (t_purdie *x, t_floatarg f) {
  x->fraction = f;
  purdie_freeReset(x);
}

void purdie_extra (t_purdie *x, t_floatarg f) {
  x->root = purdie_addNode(x->root, (int)f);
  freebytes(x->array, x->array_size);
  purdie_reset(x);
}

void *purdie_new (t_floatarg lower, t_floatarg upper, t_floatarg fraction) {
  t_purdie *x = (t_purdie *)pd_new(purdie_class);
  // leftmost inlet automatically created (and freed)
  x->in_lower = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("lower"));
  x->in_upper = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("upper"));
  x->in_fraction = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("fraction"));
  x->in_extra = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("extra"));
  x->out_series = outlet_new(&x->x_obj, &s_float);
  x->lower = (int)lower;
  x->upper = (int)upper;
  x->fraction = fraction;
  x->root = NULL;
  purdie_reset(x);
  return (void *)x;
}

void purdie_free(t_purdie *x) {
  freebytes(x->array, x->array_size);
  inlet_free(x->in_lower);
  inlet_free(x->in_upper);
  inlet_free(x->in_extra);
  inlet_free(x->in_fraction);
  outlet_free(x->out_series);
  purdie_freeTree(x->root);
}

void purdie_setup(void) {
  purdie_class = class_new(gensym("purdie"), (t_newmethod)purdie_new, (t_method)purdie_free,
    sizeof(t_purdie), CLASS_DEFAULT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
  class_addbang(purdie_class, (t_method)purdie_bang);
  class_addmethod(purdie_class, (t_method)purdie_lower, gensym("lower"), A_DEFFLOAT, 0);
  class_addmethod(purdie_class, (t_method)purdie_upper, gensym("upper"), A_DEFFLOAT, 0);
  class_addmethod(purdie_class, (t_method)purdie_fraction, gensym("fraction"), A_DEFFLOAT, 0);
  class_addmethod(purdie_class, (t_method)purdie_extra, gensym("extra"), A_DEFFLOAT, 0);
  srand((unsigned)time(NULL));
}
