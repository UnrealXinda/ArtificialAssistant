// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtificialAssistantStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateTypes.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"

FName FArtificialAssistantStyle::StyleName("FArtificialAssistantStyle");
TUniquePtr<FArtificialAssistantStyle> FArtificialAssistantStyle::Instance(nullptr);

const FName& FArtificialAssistantStyle::GetStyleSetName() const
{
	return StyleName;
}

const FArtificialAssistantStyle& FArtificialAssistantStyle::Get()
{
	if (!Instance.IsValid())
	{
		Instance = TUniquePtr<FArtificialAssistantStyle>(new FArtificialAssistantStyle);
	}
	return *(Instance.Get());
}

void FArtificialAssistantStyle::Shutdown()
{
	Instance.Reset();
}

FArtificialAssistantStyle::FArtificialAssistantStyle()
	: FSlateStyleSet(StyleName)
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ArtificialAssistant"));
	check(Plugin.IsValid());

	const FString IconsDir = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Content/Icons"));
	FSlateStyleSet::SetContentRoot(IconsDir);

	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon128x128(128.0f, 128.0f);

	Set("ArtificialAssistant.MenuIcon", new CORE_IMAGE_BRUSH_SVG("Starship/Common/ArtificialAssistant", Icon16x16));

	const FLinearColor TutorialButtonColor = FLinearColor(0.15f, 0.15f, 0.15f, 1.0f);
	const FLinearColor TutorialSelectionColor = FLinearColor(0.19f, 0.33f, 0.72f);
	const FLinearColor TutorialNavigationButtonColor = FLinearColor(0.0f, 0.59f, 0.14f, 1.0f);
	const FLinearColor TutorialNavigationButtonHoverColor = FLinearColor(0.2f, 0.79f, 0.34f, 1.0f);
	const FLinearColor TutorialNavigationBackButtonColor = TutorialNavigationButtonColor;
	const FLinearColor TutorialNavigationBackButtonHoverColor = TutorialNavigationButtonHoverColor;

	const FTextBlockStyle TutorialText = FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("Documentation.Text"))
		.SetColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor(TutorialSelectionColor);

	const FTextBlockStyle TutorialHeaderText = FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("Documentation.Header.Text"))
		.SetColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor(TutorialSelectionColor);

	Set("ArtificialAssistant.Border", new BOX_BRUSH("ArtificialAssistant/OverlayFrame", FMargin(18.0f / 64.0f), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	Set("ArtificialAssistant.Shadow", new BOX_BRUSH("ArtificialAssistant/TutorialShadow", FVector2D(256.0f, 256.0f), FMargin(114.0f / 256.0f)));

	Set("ArtificialAssistant.Highlight.Border", new BOX_BRUSH("ArtificialAssistant/TutorialBorder", FVector2D(64.0f, 64.0f), FMargin(25.0f / 64.0f)));

	const FTextBlockStyle TutorialBrowserText = FTextBlockStyle(TutorialText)
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetHighlightColor(TutorialSelectionColor);

	Set("ArtificialAssistant.Browser.Text", TutorialBrowserText);

	Set("ArtificialAssistant.Browser.WelcomeHeader", FTextBlockStyle(TutorialBrowserText)
		.SetFontSize(20));

	Set("ArtificialAssistant.Browser.SummaryHeader", FTextBlockStyle(TutorialBrowserText)
		.SetFontSize(16));

	Set("ArtificialAssistant.Browser.SummaryText", FTextBlockStyle(TutorialBrowserText)
		.SetFontSize(10));

	Set("ArtificialAssistant.Browser.HighlightTextColor", TutorialSelectionColor);

	Set("ArtificialAssistant.Browser.Button", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0.05f, 0.05f, 0.05f, 1)))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0.07f, 0.07f, 0.07f, 1)))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0.08f, 0.08f, 0.08f, 1)))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Browser.BackButton", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(1.0f, 1.0f, 1.0f, 0.0f)))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(1.0f, 1.0f, 1.0f, 0.05f)))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(1.0f, 1.0f, 1.0f, 0.05f)))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Content.Button", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0, 0, 0, 0)))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(1, 1, 1, 1)))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(1, 1, 1, 1)))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Content.NavigationButtonWrapper", FButtonStyle()
		.SetNormal(FSlateNoResource())
		.SetHovered(FSlateNoResource())
		.SetPressed(FSlateNoResource())
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Content.NavigationButton", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationButtonColor))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationButtonHoverColor))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationButtonHoverColor))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Content.NavigationBackButton", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationBackButtonColor))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationBackButtonHoverColor))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), TutorialNavigationBackButtonHoverColor))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));

	Set("ArtificialAssistant.Content.NavigationText", FTextBlockStyle(TutorialText));

	Set("ArtificialAssistant.Content.Color", FLinearColor(1.0f, 1.0f, 1.0f, 0.9f));
	Set("ArtificialAssistant.Content.Color.Hovered", FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	Set("ArtificialAssistant.Browser.CategoryArrow", new IMAGE_BRUSH("ArtificialAssistant/BrowserCategoryArrow", FVector2D(24.0f, 24.0f), FSlateColor::UseForeground()));
	Set("ArtificialAssistant.Browser.DefaultTutorialIcon", new IMAGE_BRUSH("ArtificialAssistant/DefaultTutorialIcon_40x", FVector2D(40.0f, 40.0f), FLinearColor::White));
	Set("ArtificialAssistant.Browser.DefaultCategoryIcon", new IMAGE_BRUSH("ArtificialAssistant/DefaultCategoryIcon_40x", FVector2D(40.0f, 40.0f), FLinearColor::White));

	Set("ArtificialAssistant.Browser.BackButton.Image", new IMAGE_BRUSH("ArtificialAssistant/BrowserBack", FVector2D(32.0f, 32.0f), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	Set("ArtificialAssistant.Browser.PlayButton.Image", new IMAGE_BRUSH("ArtificialAssistant/BrowserPlay", FVector2D(32.0f, 32.0f), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	Set("ArtificialAssistant.Browser.RestartButton", new IMAGE_BRUSH("ArtificialAssistant/BrowserRestart", FVector2D(16.0f, 16.0f), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	
	Set("ArtificialAssistant.Browser.Completed", new IMAGE_BRUSH("ArtificialAssistant/TutorialCompleted", Icon32x32));
	Set("ArtificialAssistant.Browser.Breadcrumb", new IMAGE_BRUSH("ArtificialAssistant/Breadcrumb", Icon8x8, FLinearColor::White));
	Set("ArtificialAssistant.Browser.PathText", FTextBlockStyle(TutorialBrowserText)
		.SetFontSize(9));

	Set("ArtificialAssistant.Navigation.Button", FButtonStyle()
		.SetNormal(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0, 0, 0, 0)))
		.SetHovered(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0, 0, 0, 0)))
		.SetPressed(CORE_BOX_BRUSH("Common/ButtonHoverHint", FMargin(4 / 16.0f), FLinearColor(0, 0, 0, 0)))
		.SetNormalPadding(FMargin(0, 0, 0, 1))
		.SetPressedPadding(FMargin(0, 1, 0, 0)));
	
	Set("ArtificialAssistant.Navigation.NextButton", new IMAGE_BRUSH("ArtificialAssistant/NavigationNext", Icon32x32));
	Set("ArtificialAssistant.Navigation.HomeButton", new IMAGE_BRUSH("ArtificialAssistant/NavigationHome", Icon32x32));
	Set("ArtificialAssistant.Navigation.BackButton", new IMAGE_BRUSH("ArtificialAssistant/NavigationBack", Icon32x32));

	Set("ArtificialAssistant.WidgetContent", FTextBlockStyle(TutorialText)
		.SetFontSize(10));

	Set("ArtificialAssistant.ButtonColor", TutorialButtonColor);
	Set("ArtificialAssistant.ButtonHighlightColor", TutorialSelectionColor);
	Set("ArtificialAssistant.ButtonDisabledColor", FAppStyle::Get().GetSlateColor("SelectionColor_Inactive"));
	
	Set("ArtificialAssistant.ContentAreaBackground", new BOX_BRUSH("ArtificialAssistant/TutorialContentBackground", FMargin(4 / 16.0f)));
	Set("ArtificialAssistant.HomeContentAreaBackground", new BOX_BRUSH("ArtificialAssistant/TutorialHomeContentBackground", FMargin(4 / 16.0f)));
	Set("ArtificialAssistant.ContentAreaFrame", new BOX_BRUSH("ArtificialAssistant/ContentAreaFrame", FMargin(26.0f / 64.0f)));
	Set("ArtificialAssistant.CurrentExcerpt", new IMAGE_BRUSH("ArtificialAssistant/CurrentExcerpt", FVector2D(24.0f, 24.0f), TutorialSelectionColor));
	Set("ArtificialAssistant.Home", new IMAGE_BRUSH("ArtificialAssistant/HomeButton", FVector2D(32.0f, 32.0f)));
	Set("ArtificialAssistant.Back", new IMAGE_BRUSH("ArtificialAssistant/BackButton", FVector2D(24.0f, 24.0f)));
	Set("ArtificialAssistant.Next", new IMAGE_BRUSH("ArtificialAssistant/NextButton", FVector2D(24.0f, 24.0f)));
	
	Set("ArtificialAssistant.PageHeader", FTextBlockStyle(TutorialHeaderText)
		.SetFontSize(22));

	Set("ArtificialAssistant.CurrentExcerpt", FTextBlockStyle(TutorialHeaderText)
		.SetFontSize(16));

	Set("ArtificialAssistant.NavigationButtons", FTextBlockStyle(TutorialHeaderText)
		.SetFontSize(16));

	// UDN documentation styles
	Set("ArtificialAssistant.Content", FTextBlockStyle(TutorialText)
		.SetColorAndOpacity(FSlateColor::UseForeground()));
	Set("ArtificialAssistant.Hyperlink.Text", FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("Documentation.Hyperlink.Text")));
	Set("ArtificialAssistant.NumberedContent", FTextBlockStyle(TutorialText));
	Set("ArtificialAssistant.BoldContent", FTextBlockStyle(TutorialText)
		.SetTypefaceFontName(TEXT("Bold")));

	Set("ArtificialAssistant.Header1", FTextBlockStyle(TutorialHeaderText)
		.SetFontSize(32));

	Set("ArtificialAssistant.Header2", FTextBlockStyle(TutorialHeaderText)
		.SetFontSize(24));
	
	Set("ArtificialAssistant.Hyperlink.Button", FButtonStyle(FAppStyle::Get().GetWidgetStyle<FButtonStyle>("Documentation.Hyperlink.Button"))
		.SetNormal(CORE_BORDER_BRUSH("Old/HyperlinkDotted", FMargin(0, 0, 0, 3 / 16.0f), FLinearColor::Black))
		.SetHovered(CORE_BORDER_BRUSH("Old/HyperlinkUnderline", FMargin(0, 0, 0, 3 / 16.0f), FLinearColor::Black)));

	Set("ArtificialAssistant.Separator", new CORE_BOX_BRUSH("Common/Separator", 1 / 4.0f, FLinearColor::Black));

	Set("ArtificialAssistant.ProgressBar", FProgressBarStyle()
		.SetBackgroundImage(CORE_BOX_BRUSH("Common/ProgressBar_Background", FMargin(5.f / 12.f)))
		.SetFillImage(CORE_BOX_BRUSH("Common/ProgressBar_NeutralFill", FMargin(5.f / 12.f)))
		.SetMarqueeImage(CORE_IMAGE_BRUSH("Common/ProgressBar_Marquee", FVector2D(20, 12), FLinearColor::White, ESlateBrushTileType::Horizontal))
	);

	// Default text styles
	{
		const FTextBlockStyle RichTextNormal = FTextBlockStyle()
			.SetFont(DEFAULT_FONT("Regular", 11))
			.SetColorAndOpacity(FSlateColor::UseForeground())
			.SetShadowOffset(FVector2D::ZeroVector)
			.SetShadowColorAndOpacity(FLinearColor::Black)
			.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f))
			.SetHighlightShape(BOX_BRUSH("Common/TextBlockHighlightShape", FMargin(3.f / 8.f)));
		Set("ArtificialAssistant.Content.Text", RichTextNormal);

		Set("ArtificialAssistant.Content.TextBold", FTextBlockStyle(RichTextNormal)
			.SetFont(DEFAULT_FONT("Bold", 11)));

		Set("ArtificialAssistant.Content.HeaderText1", FTextBlockStyle(RichTextNormal)
			.SetFontSize(20));

		Set("ArtificialAssistant.Content.HeaderText2", FTextBlockStyle(RichTextNormal)
			.SetFontSize(16));

		{
			const FButtonStyle RichTextHyperlinkButton = FButtonStyle()
				.SetNormal(CORE_BORDER_BRUSH("Old/HyperlinkDotted", FMargin(0, 0, 0, 3 / 16.0f), FLinearColor::Blue))
				.SetPressed(FSlateNoResource())
				.SetHovered(CORE_BORDER_BRUSH("Old/HyperlinkUnderline", FMargin(0, 0, 0, 3 / 16.0f), FLinearColor::Blue));

			const FTextBlockStyle RichTextHyperlinkText = FTextBlockStyle(RichTextNormal)
				.SetColorAndOpacity(FLinearColor::Blue);

			Set("ArtificialAssistant.Content.HyperlinkText", RichTextHyperlinkText);

			// legacy style
			Set("TutorialEditableText.Editor.HyperlinkText", RichTextHyperlinkText);

			const FHyperlinkStyle RichTextHyperlink = FHyperlinkStyle()
				.SetUnderlineStyle(RichTextHyperlinkButton)
				.SetTextStyle(RichTextHyperlinkText)
				.SetPadding(FMargin(0.0f));
			Set("ArtificialAssistant.Content.Hyperlink", RichTextHyperlink);

			Set("ArtificialAssistant.Content.ExternalLink", new IMAGE_BRUSH("ArtificialAssistant/ExternalLink", Icon16x16, FLinearColor::Blue));

			// legacy style
			Set("TutorialEditableText.Editor.Hyperlink", RichTextHyperlink);
		}
	}

	// Toolbar
	{
		const FLinearColor NormalColor(FColor(0xffeff3f3));
		const FLinearColor SelectedColor(FColor(0xffdbe4d5));
		const FLinearColor HoverColor(FColor(0xffdbe4e4));
		const FLinearColor DisabledColor(FColor(0xaaaaaa));
		const FLinearColor TextColor(FColor(0xff2c3e50));

		Set("TutorialEditableText.RoundedBackground", new BOX_BRUSH("Common/RoundedSelection_16x", 4.0f / 16.0f, FLinearColor(FColor(0xffeff3f3))));

		Set("TutorialEditableText.Toolbar.HyperlinkImage", new IMAGE_BRUSH("ArtificialAssistant/hyperlink", Icon16x16, TextColor));
		Set("TutorialEditableText.Toolbar.ImageImage", new IMAGE_BRUSH("ArtificialAssistant/Image", Icon16x16, TextColor));

		Set("TutorialEditableText.Toolbar.TextColor", TextColor);

		Set("TutorialEditableText.Toolbar.Text", FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
			.SetFont(DEFAULT_FONT("Regular", 10))
			.SetColorAndOpacity(TextColor)
		);

		Set("TutorialEditableText.Toolbar.BoldText", FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
			.SetFont(DEFAULT_FONT("Bold", 10))
			.SetColorAndOpacity(TextColor)
		);

		Set("TutorialEditableText.Toolbar.ItalicText", FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
			.SetFont(DEFAULT_FONT("Italic", 10))
			.SetColorAndOpacity(TextColor)
		);

		Set("TutorialEditableText.Toolbar.Checkbox", FCheckBoxStyle()
			.SetCheckBoxType(ESlateCheckBoxType::CheckBox)
			.SetUncheckedImage(CORE_IMAGE_BRUSH("Common/CheckBox", Icon16x16, FLinearColor::White))
			.SetUncheckedHoveredImage(CORE_IMAGE_BRUSH("Common/CheckBox", Icon16x16, HoverColor))
			.SetUncheckedPressedImage(CORE_IMAGE_BRUSH("Common/CheckBox_Hovered", Icon16x16, HoverColor))
			.SetCheckedImage(CORE_IMAGE_BRUSH("Common/CheckBox_Checked_Hovered", Icon16x16, FLinearColor::White))
			.SetCheckedHoveredImage(CORE_IMAGE_BRUSH("Common/CheckBox_Checked_Hovered", Icon16x16, HoverColor))
			.SetCheckedPressedImage(CORE_IMAGE_BRUSH("Common/CheckBox_Checked", Icon16x16, HoverColor))
			.SetUndeterminedImage(CORE_IMAGE_BRUSH("Common/CheckBox_Undetermined", Icon16x16, FLinearColor::White))
			.SetUndeterminedHoveredImage(CORE_IMAGE_BRUSH("Common/CheckBox_Undetermined_Hovered", Icon16x16, HoverColor))
			.SetUndeterminedPressedImage(CORE_IMAGE_BRUSH("Common/CheckBox_Undetermined_Hovered", Icon16x16, FLinearColor::White))
		);

		Set("TutorialEditableText.Toolbar.ToggleButtonCheckbox", FCheckBoxStyle()
			.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
			.SetUncheckedImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), NormalColor))
			.SetUncheckedHoveredImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), HoverColor))
			.SetUncheckedPressedImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), HoverColor))
			.SetCheckedImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), SelectedColor))
			.SetCheckedHoveredImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), HoverColor))
			.SetCheckedPressedImage(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), HoverColor))
		);

		const FButtonStyle TutorialButton = FButtonStyle()
			.SetNormal(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), NormalColor))
			.SetHovered(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), HoverColor))
			.SetPressed(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), SelectedColor))
			.SetNormalPadding(FMargin(2, 2, 2, 2))
			.SetPressedPadding(FMargin(2, 3, 2, 1));
		Set("TutorialEditableText.Toolbar.Button", TutorialButton);

		const FComboButtonStyle ComboButton = FComboButtonStyle()
			.SetButtonStyle(FButtonStyle())
			.SetDownArrowImage(IMAGE_BRUSH("Common/ComboArrow", Icon8x8))
			.SetMenuBorderBrush(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), NormalColor))
			.SetMenuBorderPadding(FMargin(0.0f));
		Set("TutorialEditableText.Toolbar.ComboButton", ComboButton);

		{
			const FButtonStyle ComboBoxButton = FButtonStyle()
				.SetNormal(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), FLinearColor::White))
				.SetHovered(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), FLinearColor::White))
				.SetPressed(BOX_BRUSH("ArtificialAssistant/FlatColorSquare", FVector2D(1.0f, 1.0f), FMargin(1), FLinearColor::White))
				.SetNormalPadding(FMargin(2, 2, 2, 2))
				.SetPressedPadding(FMargin(2, 3, 2, 1));

			const FComboButtonStyle ComboBoxComboButton = FComboButtonStyle(ComboButton)
				.SetButtonStyle(ComboBoxButton)
				.SetMenuBorderPadding(FMargin(1.0));

			Set("TutorialEditableText.Toolbar.ComboBox", FComboBoxStyle()
				.SetComboButtonStyle(ComboBoxComboButton)
			);
		}
	}

	Set("TutorialLaunch.Button", FButtonStyle()
		.SetNormalPadding(0)
		.SetPressedPadding(0)
		.SetNormal(IMAGE_BRUSH("ArtificialAssistant/TutorialButton_Default_16x", Icon16x16))
		.SetHovered(IMAGE_BRUSH("ArtificialAssistant/TutorialButton_Hovered_16x", Icon16x16))
		.SetPressed(IMAGE_BRUSH("ArtificialAssistant/TutorialButton_Pressed_16x", Icon16x16))
	);

	Set("TutorialLaunch.Circle", new IMAGE_BRUSH("ArtificialAssistant/Circle_128x", Icon128x128, FLinearColor::White));
	Set("TutorialLaunch.Circle.Color", FLinearColor::Green);

	Set("ClassIcon.EditorTutorial", new IMAGE_BRUSH_SVG("AssetIcons/EditorTutorial_16", Icon16x16));
	Set("ClassThumbnail.EditorTutorial", new IMAGE_BRUSH_SVG("AssetIcons/EditorTutorial_64", Icon16x16));

	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FArtificialAssistantStyle::~FArtificialAssistantStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}