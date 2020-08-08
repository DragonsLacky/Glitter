#include "MainCharacter.hpp"

AABB MainCharacter::box()
{
    return character[modelState == 1 || modelState == 2 ? 0 : modelState == 4 || modelState == 5 ? 3 : modelState].boundingBox;
}

MainCharacter::MainCharacter()
{
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-normal/mainCharacter.obj"));
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-walk1/mainCharacter.obj"));
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-walk2/mainCharacter.obj"));
    for (unsigned short int i = 0; i < character.size(); i++)
    {
        character[i].boundingBox.max.y = character[i].boundingBox.max.y + 0.9;
    }
    updateBoxes(glm::vec3(0.1, -0.35f, 0.0f));
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-crouch-normal/mainCharacter.obj"));
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-crouch-walk1/mainCharacter.obj"));
    character.push_back(Model("../Glitter/Resources/Objects/MainCharacter/MainCharacter-crouch-walk2/mainCharacter.obj"));
    updateBoxes(glm::vec3(0, -0.6f, -0.5f));

    character[3].boundingBox.max.x = character[3].boundingBox.max.x + 0.8;

    for (unsigned short int i = 0; i < character.size(); i++)
    {
        character[i].boundingBox.max.y = character[i].boundingBox.max.y + 1;
        character[i].boundingBox.max.z = character[i].boundingBox.max.z + 0.7;
    }
    character[3].boundingBox.min.y = character[0].boundingBox.min.y;

    modelState = 0;
    model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
}

void  MainCharacter::gravity()
{
    model = glm::translate(model, glm::vec3(0, -0.005995, 0));
    updateBoxes(glm::vec3(0, -0.005995, 0));
}

void MainCharacter::walk(walk_direction dir, float deltatime)
{
    if (framecount_walk == 80)
    {
        switch (modelState)
        {
        case 0: modelState = altWalk ? 1 : 2; altWalk = !altWalk; break;
        case 1: modelState = 0; break;
        case 2: modelState = 0; break;
        case 3: modelState = altWalk ? 4 : 5; altWalk = !altWalk; break;
        case 4: modelState = 3; break;
        case 5: modelState = 3; break;
        default:
            break;
        }
        framecount_walk = 0;
    }
    else
    {
        framecount_walk++;
    }

    if (dir == Left)
    {
        if (facingRight)
        {
            model = glm::scale(model, glm::vec3(1, 1, -1));
            //model = glm::translate(model, glm::vec3(0, 0, (box().max.x - box().min.x)));
            flipBoundingBox(true);
            facingRight = false;
        }
        model = glm::translate(model, glm::vec3(0, 0, -deltatime));
        updateBoxes(glm::vec3(-deltatime, 0, 0));
    }
    else if (dir == Right)
    {
        if (!facingRight)
        {
            model = glm::scale(model, glm::vec3(1, 1, -1));
           // model = glm::translate(model, glm::vec3(0, 0, (box().max.x - box().min.x)));
            flipBoundingBox(false);
            facingRight = true;
        }
        model = glm::translate(model, glm::vec3(0, 0, -deltatime));
        updateBoxes(glm::vec3(deltatime, 0, 0));
    }
}

void MainCharacter::flipBoundingBox(bool right)
{
    for (unsigned short int i = 0; i < character.size(); i++)
    {
        float flipBy = right ? -(character[i].boundingBox.max.x - character[i].boundingBox.min.x)/2 : (character[i].boundingBox.max.x - character[i].boundingBox.min.x)/2;
        character[i].updateBox(glm::vec3(flipBy,0,0));
    }
}

void MainCharacter::updateBoxes(glm::vec3 m)
{
    for (unsigned short int i = 0; i < character.size(); i++)
    {
        character[i].updateBox(m);
    }
}

void MainCharacter::Draw()
{
    Shader& shader = ResourceManager::GetShader("model");
    shader.setMat4("model", model);
    //std::wcout << modelState << std::endl;
    character[modelState].Draw(shader);
}

AABB& MainCharacter::attack()
{
    AABB Bbox = box();
    return facingRight? AABB(glm::vec3(Bbox.max.x, Bbox.min.y, Bbox.min.z), glm::vec3(Bbox.max.x + 1.0f, Bbox.max.y, Bbox.max.z)) : AABB(glm::vec3(Bbox.min.x - 1.0f, Bbox.min.y, Bbox.min.z), glm::vec3(Bbox.min.x, Bbox.max.y, Bbox.max.z));
}

void MainCharacter::jump()
{
    model = glm::translate(model, glm::vec3(0, 0.007995, 0));
    updateBoxes(glm::vec3(glm::vec3(0, 0.007995, 0)));
}

void MainCharacter::crouchHold()
{
    //model = glm::translate(model, glm::vec3(0, -0.01, 0));
    if (!pressedBeforeCrouch)
    {
        modelState = 3;
        pressedBeforeCrouch = true;
    }
}

void MainCharacter::crouchRelease()
{
    modelState = 0;
    pressedBeforeCrouch = false;
}

MainCharacter::~MainCharacter()
{

}