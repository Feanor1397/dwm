static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Steam",    NULL,       NULL,       1,            True,        -1 },
  { "Steam",    NULL,       "Steam",    1,            False,       -1 },
};

