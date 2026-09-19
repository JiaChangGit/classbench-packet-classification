#include "stdinc.h"
#include "ExtraList.h"
#include "FlagList.h"
#include "PortList.h"
#include "PrefixList.h"
#include "ProtList.h"
#include "TupleBST.h"
#include "dbintree.h"
#include "sbintree.h"

#include <assert.h>

// A row can contain 34 pairs: its total length, then source lengths 0..32.
// The old format string silently dropped the final source-length probability.
static void test_all_source_prefix_lengths() {
  const char* sections[] = {
      "-wc_wc", "-wc_hi", "-hi_wc", "-hi_hi", "-wc_lo", "-lo_wc",
      "-hi_lo", "-lo_hi", "-lo_lo", "-wc_ar", "-ar_wc", "-hi_ar",
      "-ar_hi", "-wc_em", "-em_wc", "-hi_em", "-em_hi", "-lo_ar",
      "-ar_lo", "-lo_em", "-em_lo", "-ar_ar", "-ar_em", "-em_ar", "-em_em"};
  FILE* seed = tmpfile();
  assert(seed != NULL);
  for (const char* section : sections) {
    fprintf(seed, "%s\n32,1", section);
    for (int source = 0; source <= 32; ++source)
      fprintf(seed, "\t%d,%d", source, source == 32 ? 1 : 0);
    fprintf(seed, "\n#\n");
  }
  rewind(seed);
  PrefixList prefixes;
  prefixes.read(seed);
  fclose(seed);

  for (int type = 0; type < 25; ++type) {
    FILE* output = tmpfile();
    assert(output != NULL);
    prefixes.print(type, output);
    rewind(output);
    char row[128];
    assert(fgets(row, sizeof(row), output) != NULL);
    assert(strcmp(row, "32,1.00000000\t32,1.00000000\n") == 0);
    fclose(output);
    const ppair selected = prefixes.choose_prefix(type, 0.5f, 0.5f);
    assert(selected.slen == 32 && selected.dlen == 0);
  }
}

// Sanitizers check nested new[]/delete[] ownership when these objects die.
static void test_array_owners() {
  PortList ports;
  ProtList protocols;
  FlagList flags;
  sbintree source_tree;
  dbintree destination_tree;
  TupleBST empty_tuples;
  TupleBST tuples;
  FiveTuple tuple = {32, 32, 1, 1, 1, 0};
  dlist* indices = tuples.Insert(&tuple);
  assert(tuples.GetTupleLists()[0] == indices);

  ExtraList unread_extras(2);
  ExtraList extras(1);
  FILE* seed = tmpfile();
  assert(seed != NULL);
  fputs("-extra\n2\n6\t2,0.5\t3,0.25\n#\n", seed);
  rewind(seed);
  extras.read(seed, 1.0f);
  assert(extras.size() == 2);
  int values[2];
  extras.choose(6, values);
  fclose(seed);
}

int main() {
  test_all_source_prefix_lengths();
  test_array_owners();
  puts("db_generator regressions passed");
}
