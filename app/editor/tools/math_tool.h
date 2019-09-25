#pragma once

namespace tools {
    //  �Ƿ������
    inline bool IsContain(const glm::vec4 & rect, const glm::vec2 & point)
    {
        return rect.x <= point.x
            && rect.y <= point.y
            && rect.x + rect.z >= point.x
            && rect.y + rect.w >= point.y;
    }

    //  �Ƿ��ڱ���
    inline int IsOnRect(const glm::vec4 & rect, const glm::vec2 & point, float border = 0)
    {
        auto t = rect.y, b = rect.y + rect.w;
        auto l = rect.x, r = rect.x + rect.z;
        if (point.x >= l && point.x <= r && std::abs(point.y - rect.y) <= border) return 0;
        if (point.x >= l && point.x <= r && std::abs(point.y - (rect.y + rect.w)) <= border) return 1;
        if (point.y >= t && point.y <= b && std::abs(point.x - rect.x) <= border) return 2;
        if (point.y >= t && point.y <= b && std::abs(point.x - (rect.x + rect.z)) <= border) return 3;
        return -1;
    }

    //  �㵽�ı��ξ���
    //      ���ؾ��뼰��ӽ��ı�
    inline std::pair<float, int> RectInDistance(const glm::vec4 & rect, const glm::vec2 & point)
    {
        auto t = std::abs(point.y - rect.y);
        auto b = std::abs(point.y - rect.y - rect.w);
        auto l = std::abs(point.x - rect.x);
        auto r = std::abs(point.x - rect.x - rect.z);
        //  ����ˮƽ��Χ��
        if (point.x >= rect.x && point.x <= rect.x + rect.z) { l = r = 0; }
        //  ���δ�ֱ��Χ��
        if (point.y >= rect.y && point.y <= rect.y + rect.w) { t = b = 0; }

        auto xmin = std::min(l, r);
        auto ymin = std::min(t, b);
        //  ������
        if (xmin == 0 && ymin == 0) { return std::make_pair(0.0f, 0); }
        //  ��ֱ����
        if (xmin == 0) { return std::make_pair(ymin, ymin == t ? 0 : 1); }
        //  ˮƽ����
        if (ymin == 0) { return std::make_pair(xmin, xmin == l ? 2 : 3); }
        //  ����Զ�ľ���
        auto dist = std::max(xmin, ymin);
        return std::make_pair(dist, dist == t ? 0
                                  : dist == b ? 1
                                  : dist == l ? 2 : 3);
    }

    //  �������Ƚ�
    inline bool Equal(const float f0, const float f1)
    {
        return std::abs(f1 - f0) <= FLT_EPSILON;
    }
}