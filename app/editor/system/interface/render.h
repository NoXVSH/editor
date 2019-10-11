#pragma once

#include "../../include.h"

class GLMesh;
class GLObject;
class GLProgram;
class GLTexture;
class GLMaterial;

namespace Interface {
    struct RenderCommand {
        std::function<void(const RenderCommand &)> mCallback;
        void Call() { if (mCallback) mCallback(*this); }
    };

    struct FowardCommand : public RenderCommand {
        SharePtr<GLMaterial> mMaterial;     //  ����
        glm::mat4 mTransform;               //  ����
    };

    struct PostCommand : public RenderCommand {
        enum TypeEnum {
            kOverlay,       //  ����
            kSwap,          //  ����
        };
        SharePtr<GLProgram> mProgram;       //  ��ɫ��
        SharePtr<GLMesh> mMesh;             //  ����
        glm::mat4 mTransform;               //  ����
        TypeEnum mType;
    };
}