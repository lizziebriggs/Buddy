using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlantPrefab : MonoBehaviour
{
    public enum Conditions { Happy, Sad, Neutral, Watered, Thirsty, Drowned, Hot, Cold, Dark, Light, Notification, Birthday, Blushing }

    [SerializeField] private AppManager appManager;
    public int listIndex;

    [Header("Condition")]
    [SerializeField] private Conditions condition;
    public float currentTemp, currentWater, currentLight;
    public float tempMin, tempMax, waterMin, waterMax, lightMin, lightMax;

    [Header("Display")]
    public Plant plantType;
    public Image plantImage, plantLEDScreen;

    [Header("Resources")]
    [SerializeField] private List<Sprite> plantConditions;
    [SerializeField] private List<Sprite> LEDScreens;

    void Start()
    {
        appManager = FindObjectOfType<AppManager>();

        if(plantConditions.Count > 0)
            plantImage.sprite = plantConditions[0];

        tempMin = plantType.recTempMin; tempMax = plantType.recTempMax;
        waterMin = plantType.recWaterMin; waterMax = plantType.recWaterMax;
        lightMin = plantType.recLightMin; lightMax = plantType.recLightMax;
    }


    void Update()
    {
        switch (condition)
        {
            case Conditions.Happy:
                plantLEDScreen.sprite = LEDScreens[0];
                break;

            case Conditions.Sad:
                plantLEDScreen.sprite = LEDScreens[1];
                break;

            case Conditions.Neutral:
                plantLEDScreen.sprite = LEDScreens[2];
                break;

            case Conditions.Watered:
                plantLEDScreen.sprite = LEDScreens[3];
                break;

            case Conditions.Thirsty:
                plantLEDScreen.sprite = LEDScreens[4];
                break;

            case Conditions.Drowned:
                plantLEDScreen.sprite = LEDScreens[5];
                break;

            case Conditions.Hot:
                plantLEDScreen.sprite = LEDScreens[6];
                break;

            case Conditions.Cold:
                plantLEDScreen.sprite = LEDScreens[7];
                break;

            case Conditions.Dark:
                plantLEDScreen.sprite = LEDScreens[8];
                break;

            case Conditions.Light:
                plantLEDScreen.sprite = LEDScreens[9];
                break;

            case Conditions.Notification:
                plantLEDScreen.sprite = LEDScreens[10];
                break;

            case Conditions.Birthday:
                plantLEDScreen.sprite = LEDScreens[11];
                break;

            case Conditions.Blushing:
                plantLEDScreen.sprite = LEDScreens[12];
                break;

            default:
                break;
        }
    }


    public void SetAsCurrentPlant()
    {
        appManager.SetNewCurrentPlant(listIndex);
    }
}
