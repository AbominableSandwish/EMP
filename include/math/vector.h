namespace Math {

    struct Vector2
    {
        float x;
        float y;

        Vector2();
        Vector2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
    };

    struct iVector2
    {
        int x;
        int y;
    };
}