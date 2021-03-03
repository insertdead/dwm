// Source: https://stackoverflow.com/a/13406408
void
setnextlayout(const Arg *arg) {
    Arg newarg = {0};

    size_t i = 0;
    while(i < LENGTH(layouts) && selmon->lt[selmon->sellt] != &layouts[i])
        i++;

    newarg.v = &layouts[(i + 1) % LENGTH(layouts)]; // you can do it without '%'
    setlayout(&newarg);
}

