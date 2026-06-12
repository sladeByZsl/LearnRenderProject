# SIGGRAPH 2012 PBR Shading Course

Source: [Practical Physically Based Shading in Film and Game Production](https://blog.selfshadow.com/publications/s2012-shading-course/#course_content)

This folder stores the complete public course materials downloaded from Self Shadow. The course is useful for the PBR-first route because it connects the math idea of physically based shading with real production choices from Far Cry 3, Disney, Pixar, and other film/game pipelines.

## Files

| Order | File | Topic |
|-------|------|-------|
| 01 | `01_hill_importance_slides.pptx` | Why physically based shading matters |
| 01 | `01_hill_importance_slides.pdf` | Why physically based shading matters |
| 02 | `02_hoffman_physics_math_slides.pdf` | PBR physics and math slides |
| 02 | `02_hoffman_physics_math_notes.pdf` | PBR physics and math notes |
| 02 | `02_hoffman_physics_math_notebook.nb` | Mathematica notebook |
| 02 | `02_hoffman_physics_math_notebook.pdf` | Notebook PDF export |
| 03 | `03_mcauley_farcry3_slides.pptx` | Far Cry 3 production PBR |
| 03 | `03_mcauley_farcry3_slides.pdf` | Far Cry 3 production PBR |
| 03 | `03_mcauley_farcry3_calibration_tool.avi` | Far Cry 3 calibration tool video |
| 03 | `03_mcauley_farcry3_notes.pdf` | Far Cry 3 notes |
| 04 | `04_gotanda_beyond_blinn_slides.pptx` | Beyond Blinn-Phong |
| 04 | `04_gotanda_beyond_blinn_slides.pdf` | Beyond Blinn-Phong |
| 04 | `04_gotanda_beyond_blinn_notes.pdf` | Beyond Blinn-Phong notes |
| 05 | `05_martinez_osl_slides.pdf` | OSL / shader language production view |
| 05 | `05_martinez_osl_notes.pdf` | OSL notes |
| 06 | `06_burley_disney_brdf_slides.key` | Disney BRDF slides |
| 06 | `06_burley_disney_brdf_slides.pdf` | Disney BRDF slides |
| 06 | `06_burley_disney_brdf_notes.pdf` | Disney BRDF notes |
| 07 | `07_smits_pixar_model_slides.key` | Pixar shading model slides |
| 07 | `07_smits_pixar_model_slides.pdf` | Pixar shading model slides |
| 07 | `07_smits_pixar_model_notebook.nb` | Pixar shading model notebook |
| 07 | `07_smits_pixar_model_notebook.pdf` | Pixar shading model notebook PDF |

## Suggested Reading Order

For a more detailed step-by-step plan, see [READING_PLAN.md](./READING_PLAN.md).

1. Start with `01_hill_importance_slides.pdf`.
   Only skim the motivation: why old shading models were hard to author consistently.

2. Read `02_hoffman_physics_math_notes.pdf` slowly.
   This is the best bridge from "PBR feels like magic" to "PBR is controlled approximations of light behavior".

3. Read `06_burley_disney_brdf_notes.pdf`.
   This is the most important practical reference for the common base color / metallic / roughness mental model.

4. Then jump to `03_mcauley_farcry3_notes.pdf`.
   Use it to see how a real game team turned PBR into artist-facing material rules.

## For Current Study

Do not try to finish all of this at once. Treat it as a long-term reference shelf:

- When learning `albedo`, `metallic`, and `roughness`, read the Disney BRDF notes first.
- When learning energy conservation, read Hoffman's physics/math notes.
- When thinking about Unity material authoring, compare with the Far Cry 3 notes.
- When reviewing older Blinn-Phong knowledge, use Gotanda's "Beyond Blinn" material.
