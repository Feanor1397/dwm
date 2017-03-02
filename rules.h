static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Steam",    "Steam",    NULL,       1,            True,        -1 },
  { "Steam",    "Steam",    "Steam",    1,            False,       -1 },
  { "Chromium", "chromium", NULL,       1<<3,         False,       -1 },
};

