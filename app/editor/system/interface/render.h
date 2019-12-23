#pragma once

#include "../../include.h"

class RawFont;
class RawMesh;
class RawImage;
class GLObject;
class RawProgram;
class RawTexture;
class RawMaterial;
class Component;

namespace interface {
    struct RenderCommand {
        std::function<void(const RenderCommand &, uint)> mCallback;
        void Call(uint pos) { if (mCallback)mCallback(*this, pos); }
    };

    //  ������Ⱦ��
    struct FowardCommand : public RenderCommand {
        enum EnabledEnum {
            kClipView = 0x1,
        };

        using TextureArray = std::vector<std::pair<std::string, SharePtr<RawTexture>>>;
        SharePtr<RawMesh>    mMesh;
        SharePtr<RawProgram> mProgram;
        TextureArray        mTextures;
        glm::mat4           mTransform;
        glm::vec4           mClipview;
        uint                mEnabled;
        FowardCommand()
            : mEnabled(0)
            , mClipview(0)
            , mTransform(0)
        { }
    };

    //  ������Ⱦ
    struct PostCommand : public RenderCommand {
        enum TypeEnum {
            kSample,        //  ����
            kSwap,          //  ����
        };
        SharePtr<RawProgram> mProgram;       //  ��ɫ��
        SharePtr<RawMesh> mMesh;             //  ����
        glm::mat4 mTransform;               //  ����
        TypeEnum mType;
    };

    //  �ֲ���Ⱦ
    struct TargetCommand : public RenderCommand {
        enum EnabledEnum {
            kTargetColor0 = 0x1,    //  �����ɫ
            kTargetColor1 = 0x2,    //  �����ɫ
            kUseCanvasSize = 0x4,   //  ʹ�û����ߴ�
        };

        enum class TypeEnum {
            kPush,
            kPop,
        };
        TypeEnum           mType;
        SharePtr<RawImage> mTexture;

        uint mEnabledFlag;
        glm::vec2 mTargetSize;
        glm::vec4 mTargetColor;

        TargetCommand()
            : mType(TypeEnum::kPush)
            , mTargetSize(0)
            , mTargetColor(0)
            , mEnabledFlag(kTargetColor0 | kTargetColor1 | kUseCanvasSize)
        { }
    };

    struct MatrixStack {
    public:
        enum class TypeEnum {
            kModel,
            kView,
            kProj,
            Length,
        };

        void Pop(TypeEnum mode)
        {
            GetStack(mode).pop();
        }

        void Push(TypeEnum mode)
        {
            GetStack(mode).push(GetStack(mode).top());
        }

        void Identity(TypeEnum mode)
        {
            GetStack(mode).push(glm::mat4(1));
        }

        void Identity(TypeEnum mode, const glm::mat4 & mat)
        {
            GetStack(mode).push(mat);
        }

        void Mul(TypeEnum mode, const glm::mat4 & mat)
        {
            Push(mode);
            GetStack(mode).top() *= mat;
        }

        const glm::mat4 & Top(TypeEnum mode)
        {
            return GetStack(mode).top();
        }

        const glm::mat4 & GetM()
        {
            return Top(TypeEnum::kModel);
        }

        const glm::mat4 & GetV()
        {
            return Top(TypeEnum::kView);
        }

        const glm::mat4 & GetP()
        {
            return Top(TypeEnum::kProj);
        }

    private:
        std::stack<glm::mat4> & GetStack(TypeEnum mode)
        {
            return _matrixs[(size_t)mode];
        }

    private:
        std::stack<glm::mat4> _matrixs[(uint)TypeEnum::Length];
    };
}