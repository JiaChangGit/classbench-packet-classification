#include "db_parser.h"
#include "trace_tools.h"

#include <assert.h>

int main() {
  FILE* rules = tmpfile();
  assert(rules != NULL);
  fputs("@10.0.0.1/32\t20.0.0.1/32\t1234 : 1234\t80 : 80\t"
        "0x06/0xFF\t0x0000/0x0000\n", rules);
  rewind(rules);
  FilterList filters;
  const int dimensions = read_filters(&filters, rules);
  assert(dimensions == 6 && filters.size() == 1);
  fclose(rules);

  // Empty input exercises the parser's early return and buffer lifetime.
  FILE* empty = tmpfile();
  assert(empty != NULL);
  FilterList no_filters;
  assert(read_filters(&no_filters, empty) == 5);
  assert(no_filters.size() == 0);
  fclose(empty);

  FILE* trace = tmpfile();
  assert(trace != NULL);
  assert(header_gen(dimensions, &filters, trace, 1, 0, 2) == 2);
  rewind(trace);
  for (int i = 0; i < 2; ++i) {
    unsigned source, destination, sport, dport, protocol, flags, filter_index;
    assert(fscanf(trace, "%u %u %u %u %u %u %u", &source, &destination,
                  &sport, &dport, &protocol, &flags, &filter_index) == 7);
    assert(source == 167772161 && destination == 335544321);
    assert(sport == 1234 && dport == 80 && protocol == 6 && filter_index == 0);
  }
  fclose(trace);
  puts("trace_generator regressions passed");
}
