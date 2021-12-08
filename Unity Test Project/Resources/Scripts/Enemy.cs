using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    // Members ************************************************************************************
    [SerializeField] private SphereSpawner m_sphereSpawner;
    [SerializeField] private Transform m_playerTransform;

    [SerializeField] private PolarTransform m_polarTransform;

    public SphereSpawner sphereSpawner
    {
        get { return m_sphereSpawner; }
        set { m_sphereSpawner = value; }
    }

    public PolarTransform polarTransform
    {
        get { return m_polarTransform; }
        set { m_polarTransform = value; }
    }

    // Methods ************************************************************************************
    public void Init(SphereSpawner s, Vector3 startPos)
    {
        m_polarTransform.vec3 = startPos;
        m_sphereSpawner = s;
        m_playerTransform = s.playerObj.transform;
        FacePlayer();
    }

    void Update()
    {
        transform.position = m_sphereSpawner.transform.position + SphereSpawner.GeneratePositionOnSphere(m_polarTransform.vec3);
        FacePlayer();
    }

    void FacePlayer()
    {
        if (m_playerTransform == null)
            return;

        transform.LookAt(m_playerTransform);
    }

    public void MoveAroundSphere(Vector3 moveDir) => MoveAroundSphere(new Vector2(moveDir.x, moveDir.y), moveDir.z);
    public void MoveAroundSphere(Vector2 orbitDir, float zoomLevel)
    {
        m_polarTransform.AddOrbitOffset(orbitDir);
        m_polarTransform.orbitRadius = m_sphereSpawner.spawnRadius + zoomLevel;
    }

    void MoveOrbit(Vector2 orbitDir)
    {
        MoveAroundSphere(orbitDir, 0);
    }

    void MoveZoom(float zoomLevel)
    {
        MoveAroundSphere(Vector2.zero, zoomLevel);
    }
}

[System.Serializable]
public class PolarTransform
{
    [SerializeField] public Vector2 orbitPosition;
    [SerializeField] public float orbitRadius;

    public Vector3 vec3
    {
        get { return new Vector3(orbitPosition.x, orbitPosition.y, orbitRadius); }
        set { orbitPosition = new Vector2(value.x, value.y); orbitRadius = value.z; }
    }

    public void AddOrbitOffset(Vector2 offset)
    {
        orbitPosition += offset*Time.deltaTime;
        while(orbitPosition.x >= 360)
            orbitPosition.x = orbitPosition.x - 360;
        while(orbitPosition.y >= 360)
            orbitPosition.y = orbitPosition.y - 360;

        while(orbitPosition.x < 0)
            orbitPosition.x = 360 + orbitPosition.x;
        while(orbitPosition.y < 0)
            orbitPosition.y = 360 + orbitPosition.y;
    }
}
