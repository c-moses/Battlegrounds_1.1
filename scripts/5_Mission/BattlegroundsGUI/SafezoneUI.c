class BuyMenuPromptUI extends UIScriptedMenu
{
    private Widget m_BuyMenu;
    private Widget m_BuyMenuPanel; 
    private TextWidget m_BuyMenuText;
    private ImageWidget m_KeyImage;
    private TextWidget m_KeyText;
    private bool m_IsVisible;

    void BuyMenuPromptUI()
    {
		m_BuyMenu   = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/SafezoneUI.layout");
        m_BuyMenuPanel   = m_BuyMenu.FindAnyWidget("SafezonePanel");
        m_BuyMenuText     = TextWidget.Cast(m_BuyMenu.FindAnyWidget("BuyMenuText") );
        m_KeyImage        = ImageWidget.Cast(m_BuyMenu.FindAnyWidget("KeyImage"));
        m_KeyText         = TextWidget.Cast(m_BuyMenu.FindAnyWidget("KeyText"));
        m_IsVisible         = false;
    }

    void ShowBuyMenuPrompt() 
    {
        m_BuyMenu.Show(true);
        m_BuyMenuText.Show(true);
        m_KeyImage.Show(true);
        string keyName = InputUtils.GetButtonNameFromInput("UABuyMenuToggle", EUAINPUT_DEVICE_KEYBOARD);
        m_KeyText.SetText(keyName);
        m_BuyMenuText.SetText("BUY MENU");
        m_IsVisible = true;
    }

    void HideBuyMenuPrompt()
    {
        if (m_BuyMenu)
        {  m_BuyMenu.Show(false);
        m_IsVisible = false; }
    }

    bool IsPromptVisible()
    { return m_IsVisible; }

    void ~BuyMenuPromptUI()
    { GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(Close); }
}