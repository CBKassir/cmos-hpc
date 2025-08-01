// ARM64 macOS assembly
// double lambertw(double k);
// fixed 10-iteration Newton-Raphson

    .text
    .global _lambertw
    .extern _log
    .extern _exp

// Constants in data section
    .data
one:
    .double 1.0

    .text
_lambertw:
    // Input: d0 = k (double)
    // Output: d0 = W(k)

    // Save k in d1
    fmov d1, d0

    // initial guess w = log(k)
    // d0 has k
    bl _log            // d0 = log(k)
    fmov d2, d0        // w = d2

    mov w3, #10        // iterations count

.loop:
    // exp(w)
    fmov d0, d2
    bl _exp            // d0 = exp(w)
    fmov d3, d0        // exp_w = d3

    // f = w * exp(w) - k
    fmul d0, d2, d3    // w * exp(w)
    fsub d0, d0, d1    // f = w*exp(w) - k
    fmov d4, d0        // f = d4

    // f' = exp(w) * (w + 1)
    ldr d5, =one
    fadd d6, d2, d5    // w + 1
    fmul d6, d3, d6    // f' = exp(w)*(w+1)

    // delta = f / f'
    fdiv d7, d4, d6

    // w = w - delta
    fsub d2, d2, d7

    subs w3, w3, #1
    bne .loop

    // Return w in d0
    fmov d0, d2
    ret