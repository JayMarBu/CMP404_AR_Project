using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(SphereSpawner)), CanEditMultipleObjects]
public class SphereSpawnerEditor : Editor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        SphereSpawner sphereSpawner = (SphereSpawner)target;
        if(GUILayout.Button("Spawn Enemy"))
        {
            sphereSpawner.SpawnObjectRaw();
        }
        if (GUILayout.Button("Clear Enemies"))
        {
            sphereSpawner.ClearEnemies();
        }

        string buttonName = (sphereSpawner.m_shouldMove) ? "disable movement" : "enable movement";

        if (GUILayout.Button(buttonName))
        {
            sphereSpawner.m_shouldMove = !sphereSpawner.m_shouldMove;
            sphereSpawner.m_sinVal = 0;
        }
    }
}
