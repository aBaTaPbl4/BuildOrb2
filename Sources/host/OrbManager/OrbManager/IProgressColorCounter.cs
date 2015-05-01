namespace OrbManager
{
    interface IProgressColorCounter
    {
        void ProcessColor(OrbColor color, string sender);
        OrbColor GetCurrentColor();

    }
}
