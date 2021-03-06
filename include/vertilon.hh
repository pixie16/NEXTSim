#ifndef VERTILON_HH
#define VERTILON_HH

namespace vertilon{

double currents[8][8][4] = {{{0.00075381, 0.00011818, 3.18E-05, 9.62E-05},
                             {0.000668493, 0.000203859, 4.61E-05, 8.15E-05},
                             {0.000577905, 0.000294447, 5.56E-05, 7.21E-05},
                             {0.000483839, 0.000388513, 6.15E-05, 6.62E-05},
                             {0.000388513, 0.000483839, 6.62E-05, 6.15E-05},
                             {0.000294447, 0.000577905, 7.21E-05, 5.56E-05},
                             {0.000203859, 0.000668493, 8.15E-05, 4.61E-05},
                             {1.18E-04, 7.54E-04, 9.62E-05, 3.18E-05}},
                            {{0.000659864, 0.000105843, 4.42E-05, 1.90E-04},
                             {0.000584638, 0.000181328, 6.87E-05, 1.65E-04},
                             {0.000505647, 0.000260319, 8.97E-05, 1.44E-04},
                             {0.000424171, 0.000341795, 0.000108205, 1.26E-04},
                             {0.000341795, 0.000424171, 0.000125829, 1.08E-04},
                             {0.000260319, 0.000505647, 0.000144353, 8.97E-05},
                             {0.000181328, 0.000584638, 0.000165362, 6.87E-05},
                             {0.000105843, 0.000659864, 0.000190136, 4.42E-05}},
                            {{0.000565918, 9.35E-05, 5.65E-05, 2.84E-04},
                             {0.000500783, 0.000158797, 9.12E-05, 2.49E-04},
                             {0.000433388, 0.000226192, 0.000123808, 2.17E-04},
                             {0.000364502, 0.000295077, 0.000154923, 1.85E-04},
                             {0.000295077, 0.000364502, 0.000185498, 1.55E-04},
                             {0.000226192, 0.000433388, 0.000216612, 1.24E-04},
                             {0.000158797, 0.000500783, 0.000249217, 9.12E-05},
                             {9.35E-05, 0.000565918, 0.000284082, 5.65E-05}},
                            {{0.000471973, 8.12E-05, 6.88E-05, 0.000378027},
                             {0.000416928, 0.000136266, 0.000113734, 0.000333072},
                             {0.000361129, 0.000192064, 0.000157936, 0.000288871},
                             {0.000304834, 0.000248359, 0.000201641, 0.000245166},
                             {0.000248359, 0.000304834, 0.000245166, 0.000201641},
                             {0.000192064, 0.000361129, 0.000288871, 0.000157936},
                             {0.000136266, 0.000416928, 0.000333072, 0.000113734},
                             {8.12E-05, 0.000471973, 0.000378027, 6.88E-05}},
                            {{0.000378027, 6.88E-05, 8.12E-05, 0.000471973},
                             {0.000333072, 0.000113734, 0.000136266, 0.000416928},
                             {0.000288871, 0.000157936, 0.000192064, 0.000361129},
                             {0.000245166, 0.000201641, 0.000248359, 0.000304834},
                             {0.000201641, 0.000245166, 0.000304834, 0.000248359},
                             {0.000157936, 0.000288871, 0.000361129, 0.000192064},
                             {0.000113734, 0.000333072, 0.000416928, 0.000136266},
                             {6.88E-05, 0.000378027, 0.000471973, 8.12E-05}},
                            {{0.000284082, 5.65E-05, 9.35E-05, 0.000565918},
                             {0.000249217, 9.12E-05, 0.000158797, 0.000500783},
                             {0.000216612, 0.000123808, 0.000226192, 0.000433388},
                             {0.000185498, 0.000154923, 0.000295077, 0.000364502},
                             {0.000154923, 0.000185498, 0.000364502, 0.000295077},
                             {0.000123808, 0.000216612, 0.000433388, 0.000226192},
                             {9.12E-05, 0.000249217, 0.000500783, 0.000158797},
                             {5.65E-05, 0.000284082, 0.000565918, 9.35E-05}},
                            {{0.000190136, 4.42E-05, 0.000105843, 0.000659864},
                             {0.000165362, 6.87E-05, 0.000181328, 0.000584638},
                             {0.000144353, 8.97E-05, 0.000260319, 0.000505647},
                             {0.000125829, 0.000108205, 0.000341795, 0.000424171},
                             {0.000108205, 0.000125829, 0.000424171, 0.000341795},
                             {8.97E-05, 0.000144353, 0.000505647, 0.000260319},
                             {6.87E-05, 0.000165362, 0.000584638, 0.000181328},
                             {4.42E-05, 0.000190136, 0.000659864, 0.000105843}},
                            {{9.62E-05, 3.18E-05, 0.00011818, 0.00075381},
                             {8.15E-05, 4.61E-05, 0.000203859, 0.000668493},
                             {7.21E-05, 5.56E-05, 0.000294447, 0.000577905},
                             {6.62E-05, 6.15E-05, 0.000388513, 0.000483839},
                             {6.15E-05, 6.62E-05, 0.000483839, 0.000388513},
                             {5.56E-05, 7.21E-05, 0.000577905, 0.000294447},
                             {4.61E-05, 8.15E-05, 0.000668493, 0.000203859},
                             {3.18E-05, 9.62E-05, 7.54E-04, 1.18E-04}}};

}

#endif
