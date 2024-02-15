// https://leetcode.com/problems/group-anagrams/
// -------------------------------------------------------------------------------------------------
// Benchmark                                                       Time             CPU   Iterations
// -------------------------------------------------------------------------------------------------
// group_anagrams_run<solution::standard>                      66459 ns        64276 ns        10453
// group_anagrams_run<solution::boost_unordered_map>           67384 ns        71498 ns         8960
// group_anagrams_run<solution::boost_unordered_node_map>      65696 ns        65569 ns        11200
// group_anagrams_run<solution::boost_unordered_flat_map>      48104 ns        49178 ns        14933
// group_anagrams_run<solution::boost_container_flat_map>     274939 ns       266841 ns         2635

#include <common.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered/unordered_node_map.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace group::anagrams {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  standard,
  boost_unordered_map,
  boost_unordered_node_map,
  boost_unordered_flat_map,
  boost_container_flat_map);
// clang-format on

template <solution Solution>
std::vector<std::vector<std::string>> run(std::vector<std::string>& strs)
{
  auto table = []() {
    if constexpr (Solution == solution::standard) {
      return std::unordered_map<std::string, std::vector<std::string>>{};
    } else if constexpr (Solution == solution::boost_unordered_map) {
      return boost::unordered_map<std::string, std::vector<std::string>>{};
    } else if constexpr (Solution == solution::boost_unordered_node_map) {
      return boost::unordered_node_map<std::string, std::vector<std::string>>{};
    } else if constexpr (Solution == solution::boost_unordered_flat_map) {
      return boost::unordered_flat_map<std::string, std::vector<std::string>>{};
    } else if constexpr (Solution == solution::boost_container_flat_map) {
      return boost::container::flat_map<std::string, std::vector<std::string>>{};
    }
  }();
  std::string key;
  for (auto& s : strs) {
    key = s;
    std::sort(key.begin(), key.end());
    table[key].push_back(std::move(s));
  }
  std::vector<std::vector<std::string>> result;
  result.reserve(table.size());
  for (auto& e : table) {
    result.push_back(std::move(e.second));
  }
  return result;
}

}  // namespace group::anagrams

#if ENABLE_TESTS || 1

TEST_CASE("group::anagrams::run")
{
  using namespace group::anagrams;
  const std::vector<std::vector<std::string>> cmp{
    { "bat" },
    { "nat", "tan" },
    { "ate", "eat", "tea" },
  };
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    std::vector<std::string> strs{ "eat", "tea", "tan", "ate", "nat", "bat" };
    const auto result = run<solution.value>(strs);
    std::vector<std::vector<std::vector<std::string>>::const_iterator> found;
    REQUIRE(result.size() == 3);
    for (auto& row : result) {
      const auto it = std::find_if(cmp.cbegin(), cmp.cend(), [size = row.size()](const auto& row) noexcept {
        return size == row.size();
      });
      REQUIRE(it != cmp.cend());
      REQUIRE(std::find(found.cbegin(), found.cend(), it) == found.cend());
      found.push_back(it);
      for (const auto& e : *it) {
        REQUIRE(std::find(row.cbegin(), row.cend(), e) != row.cend());
      }
    }
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS || 1

using group::anagrams::solution;

template <solution Solution>
void group_anagrams_run(benchmark::State& state)
{
  // clang-format off
  const std::vector<std::string> data{
    "shuffled", "lacquered", "efficacious", "michigander", "corruptness", "internals", "converter", "speeds",
    "rebellion", "transceivers", "electroencephalogram", "crematories", "bespoken", "complainant", "flotations",
    "nev", "blindfolding", "corresponds", "optionally", "aggravating", "gratifying", "healthfulness", "characterizing",
    "dole", "fantasies", "bulks", "responsibly", "exploiting", "confluences", "header", "dunno", "saddam", "adulate",
    "spoken", "bargained", "funiculars", "enlargements", "mastered", "expended", "zambians", "muggiest", "riveted",
    "junketing", "shrewish", "issachar", "wallpapered", "bridges", "efficacious", "cogitation", "parabola", "inheres",
    "song", "chock", "surfing", "windy", "richer", "shields", "rehash", "autobiographical", "idiotic", "discipline",
    "keyword", "proliferation", "hollower", "exposing", "britain", "fred", "salarying", "misplaying", "gallbladder",
    "czechoslovakia", "burying", "deprivation", "lubricated", "androids", "hurtle", "kitty", "attach", "subsidies",
    "tumbled", "unseemliest", "impelling", "surmise", "blundered", "etching", "stuccoes", "windiest", "monorail",
    "raided", "comedians", "theodora", "muhammadans", "sillies", "unlocking", "lubricating", "desperados", "vine",
    "purposeless", "calmest", "loopy", "confluences", "clings", "today", "mountaineer", "son", "axiomatic", "thur",
    "ideograph", "document", "rudolf", "joviality", "crystals", "moodiest", "footprints", "net", "taney", "crane",
    "psycho", "quantified", "aisle", "aimee", "vegetarianism", "canes", "twining", "butler", "transporters", "cohere",
    "wilts", "outlines", "imbecile", "passages", "godunov", "sunken", "maneuvers", "papyruses", "slowed", "residuals",
    "tarpaulins", "devour", "callus", "aldebaran", "wraiths", "outplay", "psychoanalyst", "flicking", "congealing",
    "unsteadier", "smoother", "bavarian", "savvy", "wino", "tortola", "stiflings", "deprecation", "iguassu",
    "surnames", "chit", "fraud", "strong", "camel", "undulate", "jiggling", "lars", "singsonging", "canny", "someway",
    "overtaken", "sonja", "rapacity", "scotch", "discus", "spill", "boated", "americanized", "phoneyed",
    "nonprofessional", "excessive", "nuisance", "haddock", "fared", "jibes", "lintels", "nurturing", "falls",
    "testimonial", "pluralism", "cookeries", "cocksure", "cassock", "appraiser", "contingent", "barbarous", "shoo",
    "groundings", "tulsa", "hughes", "fiver", "taces", "compatriot", "cockpit", "sepoy", "naughties", "topeka",
    "decadents", "rangers", "topaz", "kr", "accoladed", "palmed", "jackknifes", "overbore", "blintze", "shari",
    "corroborations", "mortgagees", "tylenol", "rockies", "caesar", "estimations", "disconnects", "coordinating",
    "satinwood", "octopus", "smithsonian", "dustiness", "subscript", "compacting", "sanctuary", "restarting",
    "palmist", "johnie", "winos", "conurbations", "contrived", "crumby", "demavend", "blooding", "electrodes",
    "composed", "wheres", "clements", "ululate", "basketball", "cattlemen", "callus", "toolboxes", "harelips",
    "garaged", "fuller", "stubborn", "scald", "devotion", "revolvers", "kernels", "lean", "adversaries", "floe",
    "uninvited", "umiaks", "crackup", "molested", "santiago", "contraltos", "bethany", "exhortations", "preferential",
    "gina", "processor", "beleaguering", "fountainhead", "politicking", "denounces", "eats", "zodiacs", "lubricated",
    "prisoning", "chautauqua", "apparently", "apiaries", "lawrence", "ellis", "vampired", "falsifiable", "shaker",
    "impecuniousness", "maurice", "vaginas", "fran", "cobain", "angkor", "discernment", "numbs", "bridges",
    "novelette", "renumbering", "multiplicand", "gluey", "tots", "garment", "outran", "disrespects", "chino",
    "pennsylvania", "puff", "chilly", "roosted", "fuses", "concede", "unimplemented", "misogynist", "disheveling",
    "wiggler", "penciling", "storage", "thoroughbreds", "copiously", "unidentifiable", "warpaths", "detriments",
    "wantoning", "welling", "philosophizes", "proprietorship", "crumbliest", "forgather", "hemlocks", "evangeline",
    "abelson", "extant", "hijacking", "repelling", "stockholder", "rebuking", "stagnates", "mechanization",
    "shenyang", "obeisance", "english", "erythrocyte", "marring", "regenerated", "spinster", "pest", "forgathered",
    "projectionist", "match", "smolder", "rhinos", "libretti", "astutely", "recuperates", "outsources", "vole",
    "maestros", "viewers", "imprecision", "astrophysicist", "aristotelian", "impressing", "picnicked", "minimalism",
    "commas", "ladled", "gobbles", "aborts", "ahem", "lira", "surreptitious", "corpses", "london", "hallucination",
    "hendricks", "traumata", "anchovy", "medication", "reexamine", "stabilization", "jackboot", "insular", "floated",
    "silkier", "entertains", "barren", "savvier", "volatile", "amethysts", "feuds", "cheddar", "cogs", "trinities",
    "underpasses", "whoopee", "cult", "housing", "fussbudgets", "laminated", "regress", "boeotian", "fugitive",
    "anthers", "nebraska", "torch", "declassify", "tijuana", "badges", "cohan", "stylish", "formosan", "lifestyles",
    "impresario", "love", "errata", "teletypewriters", "resembled", "cork", "weaver", "darlene", "preoccupied",
    "cage", "faun", "reclassifies", "confinements", "evolution", "jayne", "syndicate", "soaping", "provincials",
    "regional", "squabble", "apricot", "totes", "herbart", "beards", "carpetbagged", "assignable", "henpecks",
    "coating", "amplified", "insulation", "smooths", "parliament", "sahara", "bursitis", "lingos", "wherewithal",
    "inoffensively", "overcrowds", "bhutan", "disarrange", "zippy", "flosses", "parnell", "erratas", "sidings",
    "clapboards", "confederated", "palliative", "wirelesses", "etruscans", "neonates", "clayey", "vaccinating",
    "peskiest", "liable", "bibliographical", "squidded", "hausdorff", "lumberyard", "blythe", "pillions",
    "fiddlesticks", "sarong", "scarfed", "reformer", "gunrunning", "sweaters", "entreats", "wicca", "tennis", "quilt",
    "canisters", "frankincense", "unbar", "neighed", "cicadas", "bighorns", "tittles", "dimaggio", "costuming",
    "judas", "paints", "pastorals", "carib", "glamored", "cantering", "demotes", "currying", "excommunicating",
    "thwarting", "freebase", "niagara", "fortification", "buttercups", "survey", "barracudas"
  };
  // clang-format on
  for (auto _ : state) {
    state.PauseTiming();
    auto strs = data;
    state.ResumeTiming();
    auto result = group::anagrams::run<Solution>(strs);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(group_anagrams_run, solution::standard);
BENCHMARK_TEMPLATE(group_anagrams_run, solution::boost_unordered_map);
BENCHMARK_TEMPLATE(group_anagrams_run, solution::boost_unordered_node_map);
BENCHMARK_TEMPLATE(group_anagrams_run, solution::boost_unordered_flat_map);
BENCHMARK_TEMPLATE(group_anagrams_run, solution::boost_container_flat_map);

#endif  // ENABLE_BENCHMARKS
