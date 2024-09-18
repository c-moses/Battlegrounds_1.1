class CurrencyUI extends UIScriptedMenu
{
    private Widget m_CurrencyUI;
    private TextWidget m_CurrencyText;
    private int m_CurrencyAmount;

    override Widget Init()
    {
        m_CurrencyUI = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CurrencyUI.layout");
        m_CurrencyText = TextWidget.Cast(m_CurrencyUI.FindAnyWidget("CurrencyText"));
        return m_CurrencyUI;
    }

    void UpdateCurrencyDisplay(int amount)
    {
        m_CurrencyAmount = amount;
        string formattedCurrency = FormatCurrency(amount);
        m_CurrencyText.SetText("$" + formattedCurrency);
    }

    string FormatCurrency(int amount)
    {
        string amountStr = amount.ToString();
        string formatted = "";

        int count = 0;
        for (int i = amountStr.Length() - 1; i >= 0; i--)
        {
            formatted = amountStr[i] + formatted;
            count++;
            if (count % 3 == 0 && i != 0)
            {
                formatted = "," + formatted;
            }
        }

        return formatted;
    }
}