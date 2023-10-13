BEGIN {
OFS=", "
print "struct glasscat_record_t { const char *name; const double n; const double v; }\n%%"
}

/\yNM\y/ {
name = $2
n = $5
v = $6
print name, n, v
}