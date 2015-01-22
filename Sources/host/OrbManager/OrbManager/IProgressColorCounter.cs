namespace OrbManager
{
    interface IProgressColorCounter
    {
        void ProcessColor(OrbColor color);
        OrbColor GetCurrentColor();
    }
}
