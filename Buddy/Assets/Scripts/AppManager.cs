using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AppManager : MonoBehaviour
{
    private PlantPrefab[] buddyPlants;
    public PlantPrefab currentPlant;

    [Header("Home")]
    [SerializeField] private GameObject homePage;

    [Header("Plant Details")]
    [SerializeField] private GameObject plantDetailsPage;
    [SerializeField] private Text detailsNameToDisplay;
    [SerializeField] private Image plantToDisplay, LEDToDisplay;
    [SerializeField] private Slider tempSlider, waterSlider, lightSlider;

    [Header("Plant Information")]
    [SerializeField] private GameObject plantInfoPage;
    [SerializeField] private Text infoNameToDisplay;
    [SerializeField] private Text infoContent;

    [Header("Plant Settings")]
    [SerializeField] private GameObject plantSettingsPage;
    [SerializeField] private InputField tempMin, tempMax, waterMin, waterMax, lightMin, lightMax;

    [Header("Plant Calendar")]
    [SerializeField] private GameObject plantCalendarPage;
    [SerializeField] private Text calNameToDisplay;

    [Header("Error Box")]
    [SerializeField] private GameObject errorBox;
    [SerializeField] private Text messageText;

    void Start()
    {
        homePage.SetActive(true);
        plantDetailsPage.SetActive(false);
        plantInfoPage.SetActive(false);
        plantSettingsPage.SetActive(false);
        plantCalendarPage.SetActive(false);
        errorBox.SetActive(false);

        buddyPlants = FindObjectsOfType<PlantPrefab>();

        int index = 0;

        foreach (PlantPrefab plant in buddyPlants)
        {
            plant.listIndex = index;
            index++;
        }

        if (buddyPlants.Length > 0) currentPlant = buddyPlants[0];
    }


    void Update()
    {

    }


    public void SetNewCurrentPlant(int index)
    {
        currentPlant = buddyPlants[index];

        detailsNameToDisplay.text = currentPlant.plantType.plantName;
        plantToDisplay.sprite = currentPlant.plantImage.sprite;
        LEDToDisplay.sprite = currentPlant.plantLEDScreen.sprite;

        tempSlider.value = currentPlant.currentTemp;
        waterSlider.value = currentPlant.currentWater;
        lightSlider.value = currentPlant.currentLight;

        tempMin.text = currentPlant.tempMin.ToString(); tempMax.text = currentPlant.tempMax.ToString();
        waterMin.text = currentPlant.waterMin.ToString(); waterMax.text = currentPlant.waterMax.ToString();
        lightMin.text = currentPlant.lightMin.ToString(); lightMax.text = currentPlant.lightMax.ToString();

        infoNameToDisplay.text = currentPlant.plantType.plantName;
        infoContent.text = "About\n" + currentPlant.plantType.about + "\n\nHow To Care\n" + currentPlant.plantType.howToCare + "\n\nFamily\n" + currentPlant.plantType.family;

        calNameToDisplay.text = currentPlant.plantType.plantName;
    }

    public void DisplayError(string errorMessage)
    {
        messageText.text = errorMessage;
        errorBox.SetActive(true);
    }
}
