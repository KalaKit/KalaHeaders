# `kh_*` vs `std::string` Performance

All timings are in seconds (lower is faster).  
Iterations: ~10 million each.

| Category              | Function        | Time (s)   | std::string / CRT | Time (s)   | Speedup   |
|-----------------------|----------------|------------|-------------------|------------|-----------|
| **Checks**            | kh_empty       | 0.0473     | string empty      | 2.4074     | **50.9×** |
|                       | kh_len         | 0.0761     | string length     | 0.0479     | 0.63×     |
|                       | kh_starts      | 0.1565     | string starts_with| 3.3934     | **21.7×** |
|                       | kh_ends        | 0.1602     | string ends_with  | 3.5090     | **21.9×** |
| **Comparisons**       | kh_equals      | 0.0466     | string ==         | 0.2306     | **5.0×**  |
|                       | kh_iequals     | 0.3716     | string iequals    | 9.8774     | **26.6×** |
|                       | kh_nequals     | 0.0855     | –                 | –          | –         |
|                       | kh_niequals    | 0.3854     | –                 | –          | –         |
| **Allocation/Ownership** | kh_set      | 1.0944     | string assign     | 2.6574     | **2.4×**  |
|                       | kh_dup         | 1.0786     | string copy ctor  | 3.0287     | **2.8×**  |
|                       | kh_free        | 1.0805     | string clear      | 3.1890     | **2.9×**  |
| **Copy & Append**     | kh_copy        | 0.1177     | strncpy_s         | 0.2745     | **2.3×**  |
|                       | kh_cat         | 0.2659     | string +=         | 3.3530     | **12.6×** |
| **Search & Cleanup**  | kh_fchar       | 0.1041     | string find       | 3.3112     | **31.8×** |
|                       | kh_lchar       | 0.0811     | string rfind      | 3.3351     | **41.1×** |
|                       | kh_trim        | 0.7788     | string trim (manual)| 20.9854  | **26.9×** |
|                       | kh_remove      | 0.7003     | –                 | –          | –         |
|                       | kh_aremove     | 0.7899     | –                 | –          | –         |
| **Modification**      | kh_tolower     | 2.3724     | string tolower    | 6.7155     | **2.8×**  |
|                       | kh_toupper     | 2.3345     | string toupper    | 6.7241     | **2.9×**  |
|                       | kh_replace     | 0.1805     | –                 | –          | –         |
|                       | kh_areplace    | 0.5672     | –                 | –          | –         |