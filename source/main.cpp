#include <citro2d.h>
#include <citro3d.h>
#include <ctru.h>
#include <vector>

// Estructura para la estela de neón de la Wave
struct Point {
    float x, y;
};

int main(int argc, char* argv[]) {
    // Inicializar sistemas de la 3DS
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    
    // Pantalla inferior para información o controles táctiles
    consoleInit(GFX_BOTTOM, NULL);
    printf("\x1b[1;1H=== WAVE DASH 3DS ===");
    printf("\x1b[3;1H- Mantén A o toca la pantalla");
    printf("\x1b[4;1H  para subir en diagonal.");
    printf("\x1b[5;1H- Suelta para bajar.");
    printf("\x1b[8;1H- START: Salir del juego.");

    // Renderizador para la pantalla superior (400x240)
    C3D_RenderTarget* topScreen = C2D_CreateTarget(GFX_TOP, GFX_LEFT);

    // Variables del jugador (Wave)
    float playerX = 60.0f;
    float playerY = 120.0f;
    float waveSpeed = 3.8f;
    std::vector<Point> trail;

    // Bucle principal del juego
    while (aptMainLoop()) {
        hidScanInput();
        u32 kHeld = hidKeysHeld();
        u32 kDown = hidKeysDown();

        // Salir con START
        if (kDown & KEY_START) break;

        // Física de la Wave: Al mantener presionado A o tocar la pantalla táctil, sube. Si se suelta, baja.
        bool holding = (kHeld & KEY_A) || (kHeld & KEY_TOUCH);
        if (holding) {
            playerY -= waveSpeed;
        } else {
            playerY += waveSpeed;
        }

        // Limitar la nave dentro de la pantalla superior (0 a 240 píxeles)
        if (playerY < 8.0f) playerY = 8.0f;
        if (playerY > 232.0f) playerY = 232.0f;

        // Registrar posición para la estela de neón
        trail.push_back({playerX, playerY});
        if (trail.size() > 35) {
            trail.erase(trail.begin());
        }

        // --- RENDERIZADO ---
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(topScreen, C2D_Color32(15, 15, 30, 255)); // Fondo azul oscuro espacial
        C2D_SceneBegin(topScreen);

        // Dibujar la estela brillante de la Wave
        for (size_t i = 1; i < trail.size(); ++i) {
            // Estela con efecto degradado simulado por grosor/transparencia
            C2D_DrawLine(trail[i - 1].x, trail[i - 1].y, C2D_Color32(0, 150, 255, i * 7),
                         trail[i].x, trail[i].y, C2D_Color32(0, 200, 255, i * 7), 3.0f, 0.5f);
        }

        // Dibujar el icono de la Wave (pequeño cuadrado/flecha brillante)
        C2D_DrawRectSolid(playerX - 5, playerY - 5, 0.5f, 10, 10, C2D_Color32(0, 255, 255, 255));

        C3D_FrameEnd();
    }

    // Limpieza de memoria al salir
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
