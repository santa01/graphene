#include <Graphene.h>
#include <functional>

static bool isRunning = true;

static void mouseHandler(const SDL_Event* event) {}
static void keyHandler(const SDL_Event* event) {}

static void quitHandler(const SDL_Event* event) {
    isRunning = false;
}

int main() {
    /* Create engine instance */

    Graphene::Graphene engine(800, 600);

    /* Setup event handlers */

    auto window = engine.getWindow();
    window->onMouseEvent.connect(std::bind(mouseHandler, std::placeholders::_1));
    window->onKeyEvent.connect(std::bind(keyHandler, std::placeholders::_1));
    window->onQuitEvent.connect(std::bind(quitHandler, std::placeholders::_1));

    /* Setup scene */

    auto sceneManager = *engine.getSceneManagers().begin();
    sceneManager->setAmbientEnergy(0.2f);
    sceneManager->setLightPass(true);

    auto node1 = sceneManager->createNode();
    sceneManager->getRootNode()->attachNode(node1);

    auto node2 = sceneManager->createNode();
    sceneManager->getRootNode()->attachNode(node2);

    /* Populate scene with objects */

    auto objectManager = *engine.getObjectManagers().begin();

    auto camera = objectManager->createCamera();
    node1->attachObject(camera);

    auto light = objectManager->createLight();
    node2->attachObject(light);

    auto entity = objectManager->createEntity("assets/example.entity");
    node2->attachObject(entity);

    /* Update default viewport with camera */

    auto viewport = *engine.getWindow()->getViewports().begin();
    viewport->setCamera(camera);

    /* Update scene */

    node1->translate(2.0f, 2.0f, 3.0f);
    light->translate(0.0f, 0.0f, 2.0f);
    camera->lookAt(-2.0f, -2.0f, -3.0f);

    /* Run renderer */

    while (isRunning) {
        engine.update();
    }

    return 0;
}
