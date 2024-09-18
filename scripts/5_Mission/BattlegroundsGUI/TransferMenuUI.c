class TransferMenuUI extends UIScriptedMenu
{
    const int ID_CloseSubMenu = 7331;
    const int ID_TransferButton = 1337;
    const int ID_ReturnToMenu = 1357;
    const int ID_Category = 7531;
    const int ID_CopyID = 987;
    const int ID_SendTransfer = 899;

    private static ref TransferMenuUI m_Instance;
    protected ref BuyMenuUI m_BuyMenu;
    private Widget m_TransferRoot;
    private Widget m_SendTransferMenu;
    private ButtonWidget m_CloseTransfer;
    private ButtonWidget m_BackButton;
    private TextWidget m_CurrencyAmount;
    private GridSpacerWidget m_CategoryGrid;
    private ButtonWidget m_SellCategoryButton;
    private ButtonWidget m_SendCategoryButton;
    private ButtonWidget m_SendButton;
    private EditBoxWidget m_SteamIDEditBox;
    private EditBoxWidget m_RecAmountEditBox;
    private int m_CurrencyAmountInt;
    private string m_SteamID;
    private bool m_IsVisible = false;
    
    void TransferMenuUI() 
    { SetID(MENU_TRANSFER); }

    override Widget Init()
    {
        m_TransferRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/TransferMenuUI.layout");
        m_CloseTransfer = ButtonWidget.Cast(m_TransferRoot.FindAnyWidget("CloseTransferButton"));
        m_BackButton = ButtonWidget.Cast(m_TransferRoot.FindAnyWidget("BackButton"));
        m_CurrencyAmount = TextWidget.Cast(m_TransferRoot.FindAnyWidget("CurrencyAmount"));
        m_CategoryGrid = GridSpacerWidget.Cast(m_TransferRoot.FindAnyWidget("CategoryGrid"));

        m_SellCategoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_SellCategoryButton.FindAnyWidget("CategoryText")).SetText("SELL");

        m_SendCategoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_SendCategoryButton.FindAnyWidget("CategoryText")).SetText("SEND");

        SetCategory(1);
        UpdateCurrencyAmount();

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player) { m_SteamID = player.GetIdentity().GetPlainId(); }

        return m_TransferRoot;
    }

    void ShowTransferMenu()
    {
        if (!m_SendTransferMenu)
        {
            m_SendTransferMenu = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/SendTransferMenu.layout", m_TransferRoot);
            m_SteamIDEditBox = EditBoxWidget.Cast(m_SendTransferMenu.FindAnyWidget("SteamIDEditBox"));
            m_RecAmountEditBox = EditBoxWidget.Cast(m_SendTransferMenu.FindAnyWidget("RecAmountEditBox"));
            m_SendButton = ButtonWidget.Cast(m_SendTransferMenu.FindAnyWidget("SendButton"));
        }
    }

    void ProcessSend()
    {
        if (m_SteamIDEditBox && m_RecAmountEditBox)
        {
            string recipientSteamID = m_SteamIDEditBox.GetText();
            string amountText = m_RecAmountEditBox.GetText();
            if (IsNumber(recipientSteamID) && recipientSteamID.Length() == 17)
            {
                if (IsNumber(amountText))
                {
                    int transferAmount = amountText.ToInt();
                    if (transferAmount <= m_CurrencyAmountInt)
                    {
                        auto params = new Param2<string, int>(recipientSteamID, transferAmount);
                        GetRPCManager().SendRPC("TransferMenu", "TransferFunds", params);

                        Print("Sent " + transferAmount.ToString() + " to SteamID " + recipientSteamID)
                        m_SteamIDEditBox.SetText("");
                        m_RecAmountEditBox.SetText("");
                    }
                    else
                    {
                        Print("Not enough money to transfer");
                    }
                }
                else
                {
                    Print("The amount must contain numbers only");
                }
            }
            else
            {
                Print("Steam ID must contain exactly 17 digits");
            }
        }
        m_SteamIDEditBox.SetText("");
        m_RecAmountEditBox.SetText("");
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_SellCategoryButton)
        {
            SetCategory(0);
            return true;
        }

        else if (w == m_SendCategoryButton)
        {
            SetCategory(1);
            return true;
        }

        if (w.GetUserID() == ID_CopyID)
        {
            CopySteamIDToClipboard();
            return true;
        }

        if (w.GetUserID() == ID_SendTransfer)
        {
            ProcessSend();
            return true;
        }

        if (w.GetUserID() == ID_CloseSubMenu)
        {
            HideTransfer();
            return true;
        }

        if (w.GetUserID() == ID_ReturnToMenu)
        {
            HideTransfer();
            BuyMenuUI buyMenu = BuyMenuUI.GetInstance();
            if (buyMenu.IsMenuVisible())
            { 
                buyMenu.MenuHide(); 
            }
            
            buyMenu.MenuShow();
            buyMenu.SetCurrency(m_CurrencyAmountInt);
            return true;
        }

        return false;
    }

    // UTILITY

    void UpdateCurrencyAmount()
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
        {
            int currentCurrency = mission.GetPlayerCurrency();
            m_CurrencyAmountInt = currentCurrency;
            if (m_CurrencyAmount)
            {
                string formattedCurrency = FormatCurrency(currentCurrency);
                m_CurrencyAmount.SetText("$" + formattedCurrency);
            }
        }
    }

    void SetCurrency(int newCurrency)
    {
        m_CurrencyAmountInt = newCurrency;
        if (m_CurrencyAmount)
        {
            m_CurrencyAmount.SetText("$" + FormatCurrency(newCurrency));
        }
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

    void CopySteamIDToClipboard()
    {
        if (m_SteamID && m_SteamID.Length() > 0)
        { GetGame().CopyToClipboard(m_SteamID); }
    }

    void SetCategory(int categoryIndex)
    {
        Widget child = m_CategoryGrid.GetChildren();
        while (child)
        {
            ButtonWidget categoryButton = ButtonWidget.Cast(child);
            categoryButton.SetState(false);
            child = child.GetSibling();
        }

        if (categoryIndex == 0)
        { 
            m_SellCategoryButton.SetState(true); 
            HideTransferMenu();
        }
        else if (categoryIndex == 1)
        { 
            m_SendCategoryButton.SetState(true); 
            ShowTransferMenu();
        }
    }

    void HideTransferMenu()
    {
        if (m_SendTransferMenu)
        { 
            m_SendTransferMenu.Unlink();
            m_SendTransferMenu = null;
        }
    }

    bool IsNumber(string input)
    {
        for (int i = 0; i < input.Length(); i++)
        {
            int charAscii = input.Substring(i, 1).ToAscii();
            if (charAscii < 48 || charAscii > 57)
            { return false; }
        }
        return true;
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {        
        if (IsFocusable(w))
        {
            ColorNormal(w);
            return true;
        }
        return false;
    }

    bool IsFocusable(Widget w)
    {
        if (w && w.IsInherited(EditBoxWidget))
        { return true; }

        if (w.GetUserID() == ID_CloseSubMenu || w.GetUserID() == ID_TransferButton || w.GetUserID() == ID_ReturnToMenu || w.GetUserID() == ID_Category || w.GetUserID() == ID_CopyID || w.GetUserID() == ID_SendTransfer)
        { return true; }

        return false;
    }

    void ColorNormal(Widget w)
    {
        if (!w)
            return;

        SetFocus(null);
    }

    // SHOW/HIDE

    bool IsTransferVisible()
    { return m_IsVisible; }

    void ShowTransfer()
    {
        GetGame().GetUIManager().ShowScriptedMenu(this, NULL);
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        PPEffects.SetBlurMenu(0.3);

        OnShow();
        m_IsVisible = true;
    }

    void HideTransfer()
    {
        GetGame().GetUIManager().HideScriptedMenu(this);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().PlayerControlEnable(false);
        PPEffects.SetBlurMenu(0);

        OnHide();
        if (m_TransferRoot)
        { m_TransferRoot.Unlink(); }
        m_IsVisible = false;
    }
}