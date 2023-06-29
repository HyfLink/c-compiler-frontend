#pragma once

namespace numbers
{

    // e
    template <typename T>
    constexpr inline T e_v = T(2.718281828459045235360287471352662498L);

    /// log2(e)
    template <typename T>
    constexpr inline T log2e_v = T(1.442695040888963407359924681001892137L);

    /// log10(e)
    template <typename T>
    constexpr inline T log10e_v = T(0.434294481903251827651128918916605082L);

    /// pi
    template <typename T>
    constexpr inline T pi_v = T(3.141592653589793238462643383279502884L);

    /// 1/pi
    template <typename T>
    constexpr inline T inv_pi_v = T(0.318309886183790671537767526745028724L);

    /// 1/sqrt(pi)
    template <typename T>
    constexpr inline T inv_sqrtpi_v = T(0.564189583547756286948079451560772586L);

    /// ln(2)
    template <typename T>
    constexpr inline T ln2_v = T(0.693147180559945309417232121458176568L);

    /// ln(10)
    template <typename T>
    constexpr inline T ln10_v = T(2.302585092994045684017991454684364208L);

    /// sqrt(2)
    template <typename T>
    constexpr inline T sqrt2_v = T(1.414213562373095048801688724209698079L);

    /// sqrt(3)
    template <typename T>
    constexpr inline T sqrt3_v = T(1.732050807568877293527446341505872367L);

    /// 1/sqrt(3)
    template <typename T>
    constexpr inline T inv_sqrt3_v = T(0.577350269189625764509148780501957456L);

    /// The Euler-Mascheroni constant
    template <typename T>
    constexpr inline T egamma_v = T(0.577215664901532860606512090082402431L);

    /// The golden ratio, (1+sqrt(5))/2
    template <typename T>
    constexpr inline T phi_v = T(1.618033988749894848204586834365638118L);

    constexpr inline double e = e_v<double>;
    constexpr inline double log2e = log2e_v<double>;
    constexpr inline double log10e = log10e_v<double>;
    constexpr inline double pi = pi_v<double>;
    constexpr inline double inv_pi = inv_pi_v<double>;
    constexpr inline double inv_sqrtpi = inv_sqrtpi_v<double>;
    constexpr inline double ln2 = ln2_v<double>;
    constexpr inline double ln10 = ln10_v<double>;
    constexpr inline double sqrt2 = sqrt2_v<double>;
    constexpr inline double sqrt3 = sqrt3_v<double>;
    constexpr inline double inv_sqrt3 = inv_sqrt3_v<double>;
    constexpr inline double egamma = egamma_v<double>;
    constexpr inline double phi = phi_v<double>;

} // namespace numbers