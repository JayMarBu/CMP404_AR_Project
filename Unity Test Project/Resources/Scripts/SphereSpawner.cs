using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SphereSpawner : MonoBehaviour
{
    // Members ************************************************************************************
    [SerializeField, Min(1)] private float m_spawnRadius = 4;
    [SerializeField, Min(1)] private float m_coreRadius = 1;

    public float spawnRadius
    {
        get { return m_spawnRadius; }
        set { m_spawnRadius = value; }
    }

    [SerializeField] GizmoColours m_gizmoColours;

    [SerializeField] AngleRanges m_angles;
    private GameObject m_gameObject;

    [SerializeField] private GameObject m_player;
    [SerializeField] public float m_moveSpeed = 2;

    [SerializeField, HideInInspector] public bool m_shouldMove = false;
    [SerializeField, HideInInspector] public float m_sinVal = 0;

    public GameObject playerObj
    {
        get { return m_player; }
    }

    [SerializeField, HideInInspector] private GameObject m_enemyPrefab;
    private List<GameObject> m_enemies = new List<GameObject>();

    // Methods ************************************************************************************

    public void OnValidate()
    {
        m_enemyPrefab = Resources.Load<GameObject>("Prefabs/Test Enemy");
    }

    public void Start()
    {
        m_gameObject = SpawnObjectInternal(m_angles.angle1, m_angles.angle2);
    }

    public void Update()
    {
        if (m_shouldMove == false)
            m_gameObject.GetComponent<Enemy>().polarTransform.vec3 = new Vector3(m_angles.angle1, m_angles.angle2, m_spawnRadius);
        else
        {
            m_sinVal += Time.deltaTime*10;

            m_gameObject.GetComponent<Enemy>().MoveAroundSphere(new Vector2(m_moveSpeed * Mathf.Cos(m_sinVal), m_moveSpeed*Mathf.Sin(m_sinVal)), 0);
            Vector3 temp = m_gameObject.GetComponent<Enemy>().polarTransform.vec3;
            m_angles.angle1 = temp.x;
            m_angles.angle2 = temp.y;
        }
    }

    public void SpawnObjectRaw()
    {
        float t = Random.Range(0, 360);
        float s = Random.Range(45, 135);

        m_enemies.Add(SpawnObjectInternal(t,s));
    }

    private GameObject SpawnObjectInternal(float t = 0, float s = 0)
    {
        GameObject obj = Instantiate(m_enemyPrefab, Vector3.zero, Quaternion.identity);

        obj.GetComponent<Enemy>().Init(this, new Vector3(t,s,m_spawnRadius));
        obj.transform.position = GeneratePositionOnSphere(t,s,m_spawnRadius);

        return obj;
    }

    public static Vector3 GeneratePositionOnSphere(Vector3 pos) => GeneratePositionOnSphere(pos.x, pos.y, pos.z);
    public static Vector3 GeneratePositionOnSphere(float t = 0, float s = 0, float r = 1)
    {
        s = s * Mathf.Deg2Rad;
        t = t * Mathf.Deg2Rad;

        float x = r * Mathf.Cos(t) * Mathf.Sin(s);
        float z = r * Mathf.Sin(t) * Mathf.Sin(s);
        float y = r * Mathf.Cos(s);

        return new Vector3(x, y, z);
    }

    public void ClearEnemies()
    {
        foreach(GameObject o in m_enemies)
        {
            DestroyImmediate(o);
        }
        m_enemies.Clear();
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = m_gizmoColours.core;
        Gizmos.DrawSphere(transform.position, 0.1f);

        Gizmos.color = m_gizmoColours.spawnSphere;
        Gizmos.DrawWireSphere(transform.position, m_spawnRadius);
        if (m_gizmoColours.fillSpheres)
        {
            Gizmos.color = new Color(m_gizmoColours.spawnSphere.r, m_gizmoColours.spawnSphere.g, m_gizmoColours.spawnSphere.b, m_gizmoColours.sphereAlpha);
            Gizmos.DrawSphere(transform.position, m_spawnRadius);
        }

        Gizmos.color = m_gizmoColours.coreSphere;
        Gizmos.DrawWireSphere(transform.position, m_coreRadius);
        if (m_gizmoColours.fillSpheres)
        {
            Gizmos.color = new Color(m_gizmoColours.coreSphere.r, m_gizmoColours.coreSphere.g, m_gizmoColours.coreSphere.b, m_gizmoColours.sphereAlpha);
            Gizmos.DrawSphere(transform.position, m_coreRadius);
        }
    }
}

[System.Serializable]
public class GizmoColours
{
    [SerializeField, Range(0,1)] public float sphereAlpha = 0.25f;
    [SerializeField] public Color spawnSphere = new Color(1, 0, 0, 1);
    [SerializeField] public Color coreSphere = new Color(0, 0, 1, 1);
    [SerializeField] public Color core = Color.blue;
    [SerializeField] public bool fillSpheres = false;
}

[System.Serializable]
public class AngleRanges
{
    [SerializeField, Range(0, 360)] public float angle1;
    [SerializeField, Range(45, 135)] public float angle2;
}

